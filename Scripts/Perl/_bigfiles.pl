#
# Sample Understand perl API program 
#
# Synopsis: Reports files larger than threshold
#
# Categories: Project Report, Coding Standards
#
# Languages: All
#
# Usage:
sub usage ($)
{
    return << "END_USAGE";
${ \(shift @_) }
Usage: bigfiles.pl -db database -size size
        -db database    Specify Understand database (required for 
                        uperl, inherited from Understand)
	 -size size      Reports files with more lines than this (required)
END_USAGE
}
# 
#
#  For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl.html
# 
#  14-Dec-2000 KN
#  03-Aug-2001 DLL - updated for Understand::Gui::db()

use Understand;
use Getopt::Long;
use strict;

my $dbPath;
my $threshold;
my $help;
GetOptions(
	   "db=s" => \$dbPath,
           "size=i" => \$threshold,       
	   "help" => \$help,
          );

# help message
die usage("") if ($help);

# require arguments
die usage("-size argument is required\n\n") unless ($threshold);

# open the database
my $db = openDatabase($dbPath);


# get a list of files from the database
# for each one print it's line count if greater
# than the threshold supplied on the command line
# sort the list first so the offending files are listed
# from biggest to smallest. Note that "Countline"
# is the metric for lines in a file, not statements or comments.


my @files = sort {$b->metric("Countline") <=> $a->metric("Countline");} $db->ents("file");
foreach my $file (@files) {
    my $val = $file->metric("Countline");
   # only if metric is defined for entity
   print $file->relname(),",",$val,"\n" if $val > $threshold;
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

