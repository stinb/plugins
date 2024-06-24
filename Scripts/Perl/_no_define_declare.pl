# Synopsis: Report things that are used but never defined or declared.
#
# Categories: Project Report, Coding Standards
#
# Languages: All
#
# Description:
#              Reports any entity that is used but never defined or declared.
#
#
#  For the latest Understand perl API documentation, see
#      http://www.scitools.com/perl/
#

#--------------------------------------------------------------
sub usage($)
{
    print shift,"\n";
    print "Usage: not_declareddefined.pl\n";
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

my @ents = $db->ents();
foreach my $ent (@ents)
{
  # check if entity has any define or declare references 
  if (! $ent->refs("DeclareIn, DefineIn"))
  {
    # No define or declare found. So print entity and it's first reference, so it can be visited
    # in the GUI.
    my $ref = $ent->ref();
    print $ent->kindname," ", $ent->longname;
    print" File: ", $ref->file->longname(), " Line: ", $ref->line(), "\n" if $ref;
    print" No References.\n" if !$ref;
  }
}

print "\n Done.";

}