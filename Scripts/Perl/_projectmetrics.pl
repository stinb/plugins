# Sample Understand perl API program 
#
# Synopsis: Report project summary metrics
#
# Categories: Project Metrics
#
# Languages: All
# Usage:
sub usage($)
{
    return << "END_USAGE";
${ \( shift @_ ) }
Usage: project.pl -db database
  -db database        Specify Understand database (required for 
                      uperl, inherited from Understand)
END_USAGE
}

# Description:
#  Report Project Metrics for a given Understand project
#  Requires an existing Understand database
#
#  For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl/
#  Refer to the documenation for details on using the perl API 
#  with Understand and for details on all Understand perl API calls.
# 
# 03-May-2001 DP
# 05-Aug-2001 DLL - updated for Understand::Gui::db()
#

use Understand;
use Getopt::Long;
use strict;

my $dbPath;
my $help;
GetOptions(
	   "db=s"   => \$dbPath,
	   "help" => \$help,
	   );

# help message
die usage("") if ($help);

# open the database 
my $db=openDatabase($dbPath);

foreach my $metric( sort $db->metrics()) {
    printf "  %-25s %d\n",$metric,$db->metric($metric);
}

closeDatabase($db);



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



