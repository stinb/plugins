#
# Sample Understand perl API program
#
# Synopsis: Lists the different entity kinds in a database and how many of each
#
# Categories: Project Report, Project Metrics
#
# Languages: All
# 
# Usage:
sub usage ($)
{
    return << "END_USAGE";
${ \( shift @_ ) }
Usage: numkinds.pl -db  database 
 -db database      Specify Understand database (required for
	           uperl, inherited from Understand)
END_USAGE
}
#
# Description:
#  Lists the different entity kinds in a database and how many of each
#
#  For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl/
#  Refer to the documenation for details on using the perl API 
#  with Understand and for details on all Understand perl API calls.
# 
#  29-Dec-2000 TLB
#  05-Aug-2001 DLL - updated for Understand::Gui::db()
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


my %kinds={};
my @ents = $db->ents();
foreach my $ent (@ents)
{
  if ($ent->library() ne "Standard") 
  {
   $kinds{$ent->kindname()}++;
  }
}

foreach my $kind (sort {$a cmp $b;} keys(%kinds))
{
   if ($kinds{$kind})
   {   printf ("%s: %d\n", $kind, $kinds{$kind});   }
}

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
    $db->close() if ($dbPath);
}

