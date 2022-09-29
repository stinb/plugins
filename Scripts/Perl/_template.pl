
# Sample Understand PERL API Template Script
#
# Lists all functions

# Usage:
sub usage($) {
    return shift(@_) . <<"END_USAGE";
Usage: $0 -db database
  -db database      Specify Understand database (required for
                    uperl, inherited from Understand)
END_USAGE
}
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

#code body*******************************************************************

my @ents = $db->ents("function,method");
foreach my $ent (sort {lc($a->longname()) cmp lc($b->longname());} @ents){
  print $ent->longname. "(". $ent->parameters(1).")" . "\n";           
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
