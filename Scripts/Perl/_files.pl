#
# Sample Understand perl API program 
#
# Synopsis: Reports all project files 
#
# Categories: Project Report
#
# Languages: All
#
# Usage:
sub usage($) 
{
    return  <<"END_USAGE";
${ \( shift @_ ) }
Usage: files.pl -db database
  -db database      Specify Understand database (required for
                    uperl, inherited from Understand)
END_USAGE
}
#
#  For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl.html
# 
#  02-Oct-2001 DLL  

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
my $db = openDatabase($dbPath);


# print out all files in the database 
my $fileKinds = "File ~unknown ~unresolved";
my @files = sort {$a->longname() cmp $b->longname();} $db->ents($fileKinds); 
foreach my $file (@files) {

    # skip Ada standard library ents
    next if ($file->library() eq "Standard");

    print $file->longname() . "\n";
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

