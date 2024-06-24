#
# Demo script for certain metrics
#
# Synopsis: Outputs complexity, essential complexity, knots, essential knots
#
# Categories: Project Metrics
#
# Languages: Ada, C/C++
#
# Usage:
sub usage($) {
    return shift(@_) . <<"END_USAGE";
Usage: $0 -db database  [-alpha] [-descending]
  -db database      Specify Understand database (required)
  -alpha            Sort alphabetically by function name
  -descending       Sort in descending order by complexity value
END_USAGE
}

use Understand;
use Getopt::Long;
use strict;


#
# get options
#

my $dbPath;
my ($help, $arg_alpha, $arg_descending);
GetOptions
    ("db=s"        => \$dbPath,
     "help!"       => \$help,
	 "alpha!"      => \$arg_alpha,
	 "descending!" => \$arg_descending);
die usage("") if ($help);

# open the database 
my $db=openDatabase($dbPath);
my $language = $db->language();

# print metrics
print_metrics($db);

# close the database
closeDatabase($db);

sub print_metrics($) {
	my $db = shift;
	
	my @funcs = $db->ents("function ~unknown ~unresolved ~standard," .
						  "procedure ~unknown ~unresolved ~standard");
	print "Function/Procedure          Complexity     " . 
		  "Essential     Knots      EssentialKnots\n";
	
	my @aoh;
	foreach my $func (@funcs) {
		my %hash = ("name"       => $func->name(),
					"complexity" => $func->metric("Cyclomatic"),
					"essential"  => $func->metric("Essential"),
					"knots"      => $func->metric("Knots"),
					"essknots"   => $func->metric("MaxEssentialKnots"));
		push @aoh, \%hash;
	}
	
	@aoh = sort {$a->{"name"} cmp $b->{"name"}} @aoh if $arg_alpha;
	@aoh = sort {$b->{"complexity"} <=> $a->{"complexity"}} @aoh
		if $arg_descending;

	foreach my $func (@aoh) {
		my $p1 = 32 - length($func->{"name"});
		my $p2 = 14 - length($func->{"complexity"});
		my $p3 = 12 - length($func->{"essential"});
		my $p4 = 16 - length($func->{"knots"});
		print $func->{"name"} . (" " x $p1) . $func->{"complexity"} .
			  (" " x $p2) . $func->{"essential"} . (" " x $p3) . 
			  $func->{"knots"} . (" " x $p4) . $func->{"essknots"} . "\n";
	}
}

sub openDatabase($) {
    my ($dbPath) = @_;
    my $db = Understand::Gui::db();

    # path not allowed if opened by understand
    if ($db&&$dbPath) {
	die "database already opened by GUI, don't use -db option\n";
    }

    # open database if not already open
    if (!$db) {
	my $status;
	die usage("Error, database not specified\n") unless ($dbPath);
	($db,$status)=Understand::open($dbPath);
	die "Error opening database: ",$status,"\n" if $status;
    }
    return($db);
}


sub closeDatabase($) {
    my ($db)=@_;
    # close database only if we opened it
    $db->close() if ($dbPath);
}
