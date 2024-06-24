# Synopsis: Report things that are declared externally in code files (not H files)
#
# Categories: Project Report
#
# Languages: C
#
# Description:
#  Reports entities that are declared externally in code files. Or that are defined in another
#  code file, and declared in another code file. 
#
#  The use of this script is to enforce a requirement that externally defined items be
#  centrally declared in appropriate header files.
#  For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl/
#

#--------------------------------------------------------------
sub usage($) 
{
    print shift,"\n";
    print "Usage: c_findcodeexterns.pl\n";
    print "  -db database   Specify the Understand database if run from uperl\n";
}

use Understand;
use Getopt::Long;
use strict;

start();

#--------------------------------------------------------------

sub start()
{

my ($db, $argDb);
my $help;


GetOptions(
	   "db=s" => \$argDb,
	   "help" => \$help,
           );

# help message
die usage("") if ($help);

# open the database
  $db = Understand::Gui::db();
  if (!$db)
  {
     return usage("Error, database not specified") unless $argDb;
     my $status;
     ($db,$status) = Understand::open($argDb);
     return print("Error opening database: $status\n") if $status;
  }

# check language (this script is for C/C++ projects only)
    my $language = $db->language();
    return print("$language is currently unsupported") if ($language !=/c/i);


# get list of code files

foreach my $ent (
    sort {$a->kindname().$a->longname() cmp $b->kindname() . $b->longname();}
    $db->ents("code file"))
{
  # get list of things declared in this file

  foreach my $ref ( $ent->refs("Declare") )
  { 
  
  # if no definition, or if the definition is located in another code file, report it.
  
      if ((! $ref->ent()->ref("Definein")) || ($ref->ent()->ref("Definein")->file()->longname() ne $ent->longname() ))
      {
            
       print "\n";
       print ("\t" . $ref->ent()->kindname() . "  " . $ref->ent()->longname()); 
       printf ("\t(File: %s Line: %d)",
                $ref->file()->longname(),
                $ref->line() );
      }
  }
 
}

print "\n Done."; 

}  




