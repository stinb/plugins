# Print a comma delimited list of functions and methods with their define, declare and callby refs
#
# Language: C/C++, Java
#
# Usage:
sub usage($) {
    return shift(@_) . <<"END_USAGE";
Usage: $0 -db database
  -db database      Specify Understand database (required for
                    uperl, inherited from Understand)
END_USAGE
}
#
#  For the latest Understand perl API documentation, see
#      http://www.scitools.com/perl.html
#
#  22-Apr-2019 KG

use Understand;
use Getopt::Long;
use strict;

my $dbPath;
my $help;
GetOptions(
     "db=s" => \$dbPath,
     "help" => \$help,
          );

# help message
die usage("") if ($help);

# open the database
my $db=openDatabase($dbPath);

# check language
if ( $db->language() !~ "C|Java" ) {
    die "This script is designed for C and Java only\n";
}


#code body*******************************************************************

my @ents = $db->ents("function,method");
foreach my $ent (sort {lc($a->longname()) cmp lc($b->longname());} @ents){     
  my @refs = $ent->refs("callby, definein, declarein");
  foreach my $ref (@refs){
    print $ent->longname. "(". $ent->parameters(1)."),"; 
    print $ent->kindname.",";
    print $ref->kindname.",";
    print $ref->file->longname.",";
    print $ref->line.",";
    print $ref->scope->name."\n";
  }
  print "\n";
}

#end body********************************************************************
closeDatabase($db);


# subroutines

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
    $db->close() if $dbPath;
}

# Pass in percent complete (decimal value) to update a progress bar in the GUI or command line
sub progress{
  my $percent = shift;
  if(Understand::Gui::db()){
    Understand::Gui::progress_bar($percent);
    Understand::Gui::yield();
  }else{
    print "Progress: ".int($percent*100)."%\r";
  }
}
