#
# Sample Understand perl API program 
#
# Synopsis:  Reports inputs and outputs of a specified class to stdout
#
# Categories: Entity Report
#
# Languages: C
#
# Usage:
sub usage ($)
{
    return << "END_USAGE";
${ \(shift @_) }
Usage: class_io -db database [-name (pattern | all)]
      -db database        Specify Understand database (required for 
                          uperl, inherited from Understand)
      -name pattern | all (optional) A class name may be optionally 
                          specified.  If -name classname is specified, 
                          only that classes matching that pattern will be 
                          reported.  If -name is specified with the keyword 
                          "all", all classes will be reported. If -name 
                          option is omitted, all "root" classes will be
                          reported on.  
END_USAGE
}

# Description:
#   Shows inputs and outputs of the class - those entities defined outside the
#   class which are either used (input) or set (output) with a link to where the
#   use/set reference occurs.
#   Base classes of the specified class are not considered.
#   Requires an existing Understand for C++ database.
#   Output can be double-clicked within Understand to visit references reported on.
#
#  For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl/
#  Refer to the documenation for details on using the perl API 
#  with Understand and for details on all Understand perl API calls.
# 
# 17-Jan-2001 TLB
# 03-Aug-2001 DLL - updated for Understand::Gui::db()
#

use Understand;
use Getopt::Long;
use strict;

my $dbPath;
my $classname;
my $help;
GetOptions(
	   "db=s"   => \$dbPath,
           "name=s" => \$classname,
	   "help" => \$help,
          );

# help message
die usage("") if ($help);

# open the database 
my $db=openDatabase($dbPath);

# check language
if ( $db->language() !~ "C" ) {
    die "This script is designed for Ada only\n";
}


my @ents = ();
my @classInput = ();
my @classOutput = ();
my %unique = ();


if ($classname)
{   
   if ($classname eq "all") 
      {   @ents =  $db->ents("class ~unknown ~unresolved");
   }
   else 
      {   
         $classname = buildPattern ($classname);
         @ents = lookupEnt ($classname);  
      }
} 
else 
{
   @ents = findRootClasses();  
}

foreach my $class (sort {$a->longname() cmp $b->longname();} @ents)
{
   @classInput=();   # reset 
   @classOutput=();
   %unique = ();    
   loadClassData($class);
   reportClassData($class);
}

closeDatabase($db);


# subroutines


sub buildPattern 
{
   my ($pattern) = @_;
   
   return "" unless $pattern;
   # check for word boundaries
   if ($pattern !~ /^\*/ )  
      {$pattern = "\\b".$pattern; }
   if ($pattern !~ /\*$/ ) 
      {$pattern = $pattern."\\b"; }
  
   # replace wildcards * and ? with regex wildcards .* and ., respectively
   $pattern =~ s/\*/\.\*/g; 
   $pattern=~ s/\?/\./g; 
  
   return $pattern;
}

# returns 1 if specified entity id is already in %unique
sub duplicate
{
  my $key = @_;
  
  return 1 if $unique {$key};
  return 0;
}


# return list of all "root" classes
sub findRootClasses
{   
   my @roots=();
   foreach my $class ($db->ents("class ~unknown ~unresolved"))
   {
       if (($class->refs("base") == 0) &&
          ($class->refs("derived") >= 0) )
       {  push @roots, $class;  }
   }
   return @roots;
}


# look at refs for class and load into list if set or use is non-local
# also considers any usages by child entities defined in class
sub loadClassData
{
   my($ent) = @_;
   return unless defined($ent);
   
   foreach my $ref ($ent->refs("set ~inactive, use ~inactive, use ptr ~inactive, modify ~inactive" ) )
   {
      if ( !$ref->ent()->kind()->check("private , local, parameter") &&
           !sameClass ($ent, $ref->ent() ) )  
      {
         if ($ref->kind()->check("set, modify") ) 
         {   push(@classOutput, $ref);   }
         else # is a use
         {   push(@classInput, $ref);    }
      }
   }

   # get any declared child entities that may have sets/uses within them
   # but throw out any duplicates 
   foreach my $decl ($ent->refs("define", "class ~private, function ~private, type ~private, macro ~inactive, object ~local ~private", 1) )
   {
      if ($decl->ent()->id() !=  $ent->id())
      { 
         if (!duplicate($decl->ent()->id()))
         {
            $unique {$decl->ent()->id()} = $decl->ent();
            loadClassData ($decl->ent());
         }
      } 
   }
}

# return list of class entities matching the specified name
sub lookupEnt 
{ 
   my($entname) = @_;
   my @entlist=();

   return () unless defined($entname);

   my @allents = $db->ents("class ~unresolved ~unknown");
   foreach my $ent (@allents)
   {
       if ( ($ent->name() =~ /$entname/)  ||
            ($ent->longname() =~ /$entname/) )
       {  push @entlist, $ent;   }
    }  
    return @entlist;
}

# print data in global classInput and classOutput lists
sub reportClassData 
{
   my($class) = @_;
   my ($first, $lastent);
   
   return unless defined($class);
   
   # report class name and associated comments, if any
   print "\n".$class->kindname()." ".$class->longname();
   my @decl = $class->refs("Definein");
   if (@decl) 
   { 
      print " \t[File: ".$decl[0]->file()->longname()." Line: ".$decl[0]->line()."]\n"; 
   }   
   if ($class->comments())  { print $class->comments()."\n\n"; }


   # report inputs
   $lastent  = "";
   if (@classInput > 0) 
   {
      foreach my $ref (sort {$a->ent()->longname() cmp $b->ent()->longname;} @classInput) 
      {
          if ($ref->ent()->longname() ne $lastent)
          {
             if ($lastent eq "") { print  "\n  INPUTS:\n"; }
             print  "      ".$ref->ent()->longname();
             $lastent = $ref->ent()->longname();
             print "\n";
             @decl = $ref->ent()->refs("Definein");
             if (@decl)
             { 
                print "          Declared: ".$decl[0]->ent()->longname()." \t[File: ".$decl[0]->file()->longname()." Line: ".$decl[0]->line()."]\n";
             }
          }     
          print "          ".$ref->kindname().": ".$ref->scope()->name()." \t[File: ".$ref->file()->longname()." Line: ".$ref->line()."]\n";
       }
    }
    else
       {  print  "  INPUTS:   None\n"; }
       
   # report outputs
   $lastent  = "";
   if (@classOutput > 0)
   {
      foreach my $ref (sort {$a->ent()->longname() cmp $b->ent()->longname;} @classOutput) 
      {
          if ($ref->ent()->longname() ne $lastent)
          {
             if ($lastent eq "") { print "\n  OUTPUTS:\n"; }
             print "      ".$ref->ent()->longname();
             $lastent = $ref->ent()->longname();
             print "\n";
             @decl = $ref->ent()->refs("Definein");
             if (@decl)
             { 
                print "          Declared: ".$decl[0]->ent()->longname()." \t[File: ".$decl[0]->file()->longname()." Line: ".$decl[0]->line()."]\n";
             }
          }     
          print "          ".$ref->kindname().": ".$ref->scope()->name()." \t[File: ".$ref->file()->longname()." Line: ".$ref->line()."]\n";
       }
    }
    else
         {  print  "  OUTPUTS:  None\n"; }


}

# return true if both entities are in same class
sub sameClass
{
   my ($e1, $e2) = @_;
   my (@n1,@n2);
   
   return 0 unless (defined($e1) || defined($e2));
   
   if ($e1->id() == $e2->id())  
      { return 1; }

   @n1 = split(/\::/, $e1->longname()); 
   @n2 = split(/\::/, $e2->longname());
   if ($n1[0] eq $n2[0]) 
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

