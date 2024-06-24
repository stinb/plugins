#
# Sample Understand perl API program 
#
#	Synopsis: Writes package input/output information  
#   	Note: long type strings are truncated to $max_type_len (256)
#   	Requires an existing Understand for Ada database
#   	Output can be used within Understand to visit references reported on.
#   	Standard library entities (and references to them) are ignored
#
# Categories: Project Report, Entity Report
#
# Languages: Ada
#
# Usage:
sub usage($) 
{
    return << "END_USAGE";
${ \( shift @_ ) }
Usage: pkg_io -db database [-name (packagename] [-outfile filename]
 -db database        Specify Understand database (required for 
                     uperl, inherited from Understand)
 -name packagename   (optional) Only packages matching that specified 
                     pattern will be reported  To report all packages,
                     specify -name "*".  If the -name option is not
                     specified, all root packages will be reported (default).
 -outfile filename   Specifiy output file instead of stdout.
END_USAGE
}

#  For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl/
#  Refer to the documenation for details on using the perl API 
#  with Understand and for details on all Understand perl API calls.
# 
#  05-Jan-2001 TLB
#  05-Aug-2001 DLL - updated for Understand::Gui::db()
#  03-Dec-2003 DLL - output files of type .html or .htm get html
#                    header /trailer and <pre> for formatting text
#

use Understand;
use Getopt::Long;
use strict;

my $dbPath;
my $pkgname;
my $outfile;
my $help;
GetOptions(
	   "db=s"   => \$dbPath,
           "name=s" => \$pkgname,
           "outfile=s" => \$outfile,
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

# use optional output file
my $htmlOutput;
if ($outfile) {

    $htmlOutput = 1 if ( $outfile =~ /.htm[l]*$/ );

    open(RPT, "> $outfile") or die "Can't open output file $outfile: $!\n";
    print "Writing output to $outfile\n";
    select(RPT);

    print "<html><body><pre>\n" if ($htmlOutput);
}

my $max_type_len = 256; 
my @ents;

if ($pkgname)
{   
   if ($pkgname eq "*") 
      { @ents = findAllPkgs(); }
   else 
      { @ents = lookupEnt ($pkgname); }
} 
else 
{
   @ents = findRootPkgs();  
}

my @pkgInput=();
my @pkgOutput=();
my %unique = ();    

foreach my $pkg (sort {$a->longname() cmp $b->longname();} @ents)
{
   @pkgInput=();   # reset global arrays
   @pkgOutput=();
   %unique = ();    
   loadPkgData($pkg);
   reportPkgData($pkg);
}

print "</pre></body></html>\n" if ($htmlOutput);
close (RPT);
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
   my @allents = $db->ents("package ~formal, task");
   foreach my $ent (sort {$a->longname() cmp $b->longname();} @allents)
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
      if ( ( $ref->ent()->library() ne "Standard") and      # ignore refs to std lib entities
           ((!samePkg ($ref->ent(), $ent)) ||     
            ($ref->ent()->kind()->check("ada constant ~local, ada object ~local ~task ~protected") ))  )
      {
         if ($ref->kind()->check("set")) 
         { 
            push(@pkgOutput, $ref);
         }
         else # is a use
         {
             push(@pkgInput, $ref); 
         }
      }
   }

   # get any declared child entities that may have sets/uses within them
   # could be program units, or types, even objects
   # but throw out any duplicates  (TBD - can use unique flag after B96 release)
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
  
   my @allents = $db->ents("package, task");
   foreach my $ent (@allents)
   {
      if ($ent->library() ne "Standard") 
      {  
         if ( ($ent->name() =~ /$entname/i) ||
              ($ent->longname() =~ /$entname/i) )
            {  push @entlist, $ent;   }
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
   if ((@pkgInput > 0) or (@pkgOutput > 0))
   {
      print "\n".$pkg->kindname()." ".$pkg->longname();
      my @pkgdecl = getDeclareinRef($pkg);
      if (@pkgdecl) 
      { 
         print " \t[File: ".$pkgdecl[0]->file()->longname()." Line: ".$pkgdecl[0]->line()."]\n"; 
      }   
      if ($pkg->comments())  { print $pkg->comments()."\n\n"; }
   }

   # report inputs
   $lastent  = "";
   foreach my $ref (sort {$a->ent()->longname() cmp $b->ent()->longname;} @pkgInput) 
   {
       if (lc($ref->ent()->longname()) ne $lastent)
       {
          if ($lastent eq "") { print "\n  INPUTS:\n"; }
          print "      ".$ref->ent()->longname();
          $lastent = lc($ref->ent()->longname());
          if ($ref->ent()->type())
          {
             if (length($ref->ent()->type()) > $max_type_len)
                {  printf "  (%.*s...)", $max_type_len, $ref->ent()->type();   }
             else
                {  print "  (".$ref->ent()->type().")"; }
          }
          print "\n";
          my @decl = getDeclareinRef($ref->ent());
          if (@decl)
          { 
             print "          Declared: ".$decl[0]->ent()->longname()." \t[File: ".$decl[0]->file()->longname()." Line: ".$decl[0]->line()."]\n";
          }
       }     
       print "          ".$ref->kindname().": ".$ref->scope()->name()." \t[File: ".$ref->file()->longname()." Line: ".$ref->line()."]\n";
    }

   # report outputs
   $lastent  = "";
   foreach my $ref (sort {$a->ent()->longname() cmp $b->ent()->longname;} @pkgOutput) 
   {
       if (lc($ref->ent()->longname()) ne $lastent)
       {
          if ($lastent eq "") { print "\n  OUTPUTS:\n"; }
          print "      ".$ref->ent()->longname();
          $lastent = lc($ref->ent()->longname());
          if ( $ref->ent()->type())
          {
             if (length($ref->ent()->type()) > $max_type_len)
                {  printf "  (%.*s...)", $max_type_len, $ref->ent()->type();   }
             else
                {  print "  (".$ref->ent()->type().")"; }
          }
          print "\n";
          my @decl = getDeclareinRef($ref->ent());
          if (@decl)
          { 
             print "          Declared: ".$decl[0]->ent()->longname()." \t[File: ".$decl[0]->file()->longname()." Line: ".$decl[0]->line()."]\n";
          }
       }     
       print "          ".$ref->kindname().": ".$ref->scope()->name()." \t[File: ".$ref->file()->longname()." Line: ".$ref->line()."]\n";
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



