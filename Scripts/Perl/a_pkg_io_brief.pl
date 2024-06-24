#
# Sample Understand perl API program
#
# Synopsis : Lists which packages are used as inputs or outputs to specified packages.
#
# Categories: Entity Report
#
# Languages: Ada
#
# Usage:
sub usage($)
{
    return << "END_USAGE";
${ \( shift @_ ) }
Usage: pkg_io_brief.pl -db database [-name packagenamepattern] 
 -db database        Specify Understand database (required for 
                     uperl, inherited from Understand)
 -name packagename   (optional) Specify the package name pattern.  For
                     all packages, specify "*".  If -name option is not
                     specified, all root packages will be reported (default).
END_USAGE
}

# Description:
#   Lists packages which contain inputs/outputs to the specified package
#   Requires an existing Understand for Ada database
#   Output can be used within Understand to visit references reported on.
#   Standard library entities (and references to them) are ignored
#
#  For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl/
#  Refer to the documenation for details on using the perl API 
#  with Understand and for details on all Understand perl API calls.
# 
#  02-Feb-2001 TLB
#  05-Aug-2001 DLL - updated for Understand::Gui::db()
#

use Understand;
use Getopt::Long;
use strict;

my $dbPath;
my $pkgname;
my $help;
GetOptions(
	   "db=s"   => \$dbPath,
           "name=s" => \$pkgname,
	   "help" => \$help,
	   );

# help message
die usage("") if ($help);

# open the database 
my $db=openDatabase($dbPath);

# check language
if ( $db->language() !~ "Ada" ) {
    die "This script is designed for Ada only\n";
}


my @ents;
if ($pkgname)
{   
   if ($pkgname eq "*") 
      {   @ents = findAllPkgs();   }
   else 
      {   @ents = lookupEnt ($pkgname);   }
} 
else 
{
   @ents = findRootPkgs();  
}

my %pkgInput=();   
my %pkgOutput=();
my %unique = ();    
foreach my $pkg (sort {lc($a->longname()) cmp lc($b->longname());} @ents)
{
   %pkgInput=();   # reset 
   %pkgOutput=();
   %unique = ();    
   loadPkgData($pkg);
   reportPkgData($pkg);
}

closeDatabase($db);


# subroutines


# returns 1 if specified entity id is already in %unique
sub duplicate
{
  my $key = @_;
  
  return 1 if $unique {$key};
  return 0;
}

# return list of all packages and tasks not in the standard lib
sub findAllPkgs
{
   my @list=();
   foreach my $ent ( $db->ents("package ~formal, task"))
   {
       if ($ent->library() ne "Standard") 
       {
           push (@list, $ent); 
       }
   }
   return @list;
}

# return list of all "root" entities
# ignoring any duplicates
sub findRootPkgs
{
   my %roothash = ();
   
   my @allfiles = $db->ents("file ~unresolved");
   foreach my $file (@allfiles)
   {
      if ($file->library() ne "Standard") 
      {
         foreach my $root ( $file->refs("root", "",1)) 
         {  
            if ( not defined( $roothash {$root->ent()->id()}) )
            {
               $roothash {$root->ent()->id()} = $root->ent();
            }
         }
      }
   }
   return values (%roothash)
}

# get declarein reference of specified entity
# there may be several, so get "best"
sub getDeclareinRef 
{
   my ($ent) =@_;
   return () unless defined($ent);
   
   # if a rename, get the decl of the rename entity instead 
   my @renameRefs;
   if (@renameRefs = $ent->refs("rename") )
   { $ent = $renameRefs[0]->ent(); }
   
   my @decl=();   
   my @declOrder = ("declarein ~spec ~body ~instance ~formal ~incomplete ~private ~stub",
                 "spec declarein",
                 "body declarein",
                 "instance declarein",
                 "formal declarein",
                 "incomplete declarein",
                 "private declarein",
                 "stub declarein");
               
   foreach my $type (@declOrder)
   {
      @decl = $ent->refs($type);
      if (@decl) { last;  }
   }
   return @decl;
}


# look at refs for package and load into list if set or use is outside the package
# also considers any usages by child entities defined in package
sub loadPkgData
{
   my($ent) = @_;
   return unless defined($ent);
   
   foreach my $ref ($ent->refs("set,use" ) )
   {
      my @renames = $ref->ent()->refs("rename");
      my $realEnt;
      if (@renames)
      {  $realEnt = $renames[0]->ent();  }
      else
      {  $realEnt = $ref->ent();  }
      
      if ( ( $ref->ent()->library() ne "Standard") and      # ignore refs to std lib entities
           ((!samePkg ($realEnt, $ent)) ||     
            ($realEnt->kind()->check("ada constant ~local, ada object ~local ~task ~protected") ))  )
      {
         my $pkgName = getPkgName($realEnt);
         if ($ref->kind()->check("set")) 
         { 
             $pkgOutput{$pkgName} = 1;
         }
         else # is a use
         {            
            $pkgInput{$pkgName} = 1;  
         }
      }
   }

   # get any declared child entities that may have sets/uses within them
   # could be program units, or types, even objects
   # but throw out any duplicates  
   foreach my $decl ($ent->refs("declare", "function ~formal, procedure ~formal, package ~formal, task, type, subtype, object ~local ~task ~protected", 1) )
   {
      if ($decl->ent()->id() !=  $ent->id())
      { 
         if (!duplicate($decl->ent()->id()))
         {
            $unique {$decl->ent()->id()} = $decl->ent();
            loadPkgData ($decl->ent());
         }
      } 
   }
}

# return list of package or task entities matching the specified name pattern
sub lookupEnt 
{ 
   my($entname) = @_;
   my @entlist=();

   return () unless defined($entname);

   # check for word boundaries
   if ($entname !~ /^\*/ )  
      {$entname = "\\b".$entname; }
   if ($entname !~ /\*$/ ) 
      {$entname = $entname."\\b"; }
  
   # replace wildcards * and ? with regex wildcards .* and ., respectively
   $entname =~ s/\*/\.\*/g; 
   $entname =~ s/\?/\./g; 
   my @allents = $db->ents("package ~formal, task");
   foreach my $ent (@allents)
   {
      if ($ent->library() ne "Standard") 
      {  
         if ( ($ent->name() =~ /$entname/i) ||
              ($ent->longname() =~ /$entname/i) )
            {   push @entlist, $ent;   }
      }
    }  
    return @entlist;
}

# print data in global pkgInput and pkgOutput lists
sub reportPkgData 
{
   my($pkg) = @_;
   my ($first, $lastent);
   
   return unless defined($pkg);
   
   # report package name and associated comments, if any
   if (%pkgInput or %pkgOutput)
   {
      print "\n".$pkg->kindname()." ".$pkg->longname();
      my @pkgdecl = getDeclareinRef($pkg);
      if (@pkgdecl) 
      { 
         print " \t[ File: ".$pkgdecl[0]->file()->longname()." Line: ".$pkgdecl[0]->line()." ]\n"; 
      }   
      if ($pkg->comments())  { print $pkg->comments()."\n\n"; }
   }

   # report inputs
   if (%pkgInput) { print "\n  INPUTS:\n"; }
   foreach my $key (sort {lc($a) cmp lc($b)} keys (%pkgInput)) 
   {
      print "      ".$key."\n";
   }

   # report outputs
   if (%pkgOutput) { print "\n  OUTPUTS:\n"; }
   foreach my $key (sort {lc($a) cmp lc($b)} keys (%pkgOutput)) 
   {
          print "      ".$key."\n";
   }
}

# return true if both entities are in same package
sub samePkg
{
   my ($e1, $e2) = @_;
   my (@n1,@n2);
   
   return 0 unless (defined($e1) || defined($e2));
   
   if ($e1->id() == $e2->id())  
      { return 1; }

   @n1 = split(/\./, $e1->longname()); 
   @n2 = split(/\./, $e2->longname());
   
   if (lc($n1[0]) eq lc($n2[0])) 
      { return 1; }

   return 0;
}

# return (root) package name of entity or empty string
sub getPkgName 
{
   my ($ent) = @_;
   return "" unless (defined($ent));
   my @name = split(/\./, $ent->longname()); 
   
   if (@name)
   {   return @name[0];   }
   
   return $ent->longname();
   
   
}


sub openDatabase($)
{
    my ($dbPath) = @_;
    
    my $db = Understand::Gui::db();

    # path not allowed if opened by understand
    if ($db&&$dbPath) {
	die "database already opened by GUI, don't use -db option\n";
    }

    # open database if not already open
    if (!$db) {
	my $status;
	die usage("Error, database not specified\n\n") unless ($dbPath);
	($db,$status)=Understand::open($dbPath);
	die "Error opening database: ",$status,"\n" if $status;
    }
    return($db);
}

sub closeDatabase($)
{
    my ($db)=@_;

    # close database only if we opened it
    $db->close() if ($dbPath);
}



