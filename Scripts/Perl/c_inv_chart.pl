#
# Sample Understand perl API program 
#
# Synopsis: Output invocation tree in GraphViz's input format
#
# Categories: Project Report, Graphic
#
# Languages: C
# Usage:
sub usage ($)
{
    return << "END_USAGE";
${ \( shift @_ ) }
Usage: inv_chart -db database [-known]
 -db database     (required) Specify an Understand for C database.
 -known           (optional) Specify to include only known project functions
                  in the invocation graph.  Any functions whose definitions
                  are not in the project will be omitted. By default all
                  functions (known and unknown) are shown in the graph.
END_USAGE
}

# Description: Outputs invocation information into graphViz's input format.
#        GraphViz (http://www.graphviz.org) is an open-source graphing tool. 
#
#  For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl/
#  Refer to the documenation for details on using the perl API 
#  with Understand and for details on all Understand perl API calls.
# 
#
use Understand;
use Getopt::Long;
use strict;

my $dbPath;
my $known;
my $help;
GetOptions(
	   "db=s" => \$dbPath,
           "known" => \$known,
	   "help" => \$help,
           );

# help message
die usage("") if ($help);

# open the database 
my $db=openDatabase($dbPath);

# check language
if ( $db->language() !~ "C" ) {
    die "This script is designed for C only\n";
}

print "digraph G {\n";
print "rankdir=LR;\n";
print "ratio=fill;\n";

foreach my $func ($db->ents("function ~unknown ~unresolved")) {
    my @calls;
    if ($known)
      { @calls = $func->ents("call", "function ~unknown ~unresolved"); }
    else
      { @calls = $func->ents("call");  }
    foreach my $call (@calls) {
	print "\"",$func->longname(),"\"","->","\"",$call->longname(),"\"",";\n";
    }
}
print "}\n";

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


