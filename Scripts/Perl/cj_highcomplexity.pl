#
# Sample Understand PERL API Script
#
# Synopsis: Lists functions with high complexity (C/C++)
#
#   Lists all functions with a complexity higher than threshold.
#   Requires an existing Understand for C++ database
#
# Categories: Project Report, Project Metrics
#
# Languages: C, Java
#
# Usage:
sub usage ($)
{
    return << "END_USAGE";
${ \( shift @_ ) }
Usage: highcomplexity.pl -db database -min threshold -use whichComplexity
  -db database      Specify Understand database (required for
	            uperl, inherited from Understand)
  -min threshold    Minimum complexity to report
  -use  Cyclomatic (default), CyclomaticModified or CyclomaticStrict
END_USAGE
}

#  For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl.html
# 
# 14-Dec-2000 KN
# 04-Aug-2001 DLL - updated for Understand::Gui::db()


use Understand;
use Getopt::Long;
use strict;

my $dbPath;
my $threshold;
my $help;
my $whichMetric = "Cyclomatic";
GetOptions(
	   "db=s" => \$dbPath,
           "min=i" => \$threshold,       
	   "help" => \$help,
	   "use=s" => \$whichMetric
          );

# help message
die usage("") if ($help);

#verify valid complexity metric name

die usage("") if ($whichMetric !~ /cyclomatic||cyclomaticmodified||cyclomaticstrict/i);

# open the database 
my $db=openDatabase($dbPath);

# check language
if ( $db->language() !~ /c|java/i ) {
    die "This script is designed for C/C++ and Java only\n";
}


# get a list of functions from the database
# for each one print it's complexity if greater
# than the threshold supplied on the command line
# sort the function list first so the functions appear
# from highest complexity to lowest.

my @funcs = sort {$b->metric($whichMetric) <=> $a->metric($whichMetric);} $db->ents("Function,method");
foreach my $func (@funcs) {
 if ($func->library() !~ /standard/i)
 {
    my $val = $func->metric($whichMetric);
    # only if metric is defined for entity
    if ($val > $threshold)
    {
		my $def = $func->ref("definein");
		print $func->longname(),",",$val;
		if ($def){
			print ",\t\tFile: ",$def->file->longname()," Line:",$def->line,"\n";
		}
    }
 }
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

