# Understand perl API program: missing_includes.pl 
#
# Synopsis: reports (possibly missing) include files that make no declarations
#
# Categories: Project Report, Coding Standards
#
# Languages: Ada, Fortran, C
#
# Usage:
sub usage ($)
{
	return << "END_USAGE";
${ \( shift @_ ) }
Usage: $0 -db database
	-db database   An Understand database for C, C++, Ada or Fortran.
	-kind kind     [optional] show only references to the specified kind(s)
END_USAGE
}

# Description:
#    Lists all entities used but not declared.
#
#
#  For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl/
#  Refer to the documenation for details on using the perl API 
#  with Understand and for details on all Understand perl API calls.
# 
#  16-Nov-01 Evan Knop
#  14-Feb-2014 AL

use strict;
use Understand;
use Getopt::Long;


my $language;

my $dbPath;
my $help;
my @kinds;
GetOptions(
	"db:s" => \$dbPath,
	"help" => \$help,
	"kind|kinds:s" => \@kinds,
	);

# do help option
die usage("") if $help;

my $db = openDatabase($dbPath);

#code body*******************************************************************

my @entities = $db->ents("file unresolved");

foreach my $ent (sort {$a->longname() cmp $b->longname()} @entities) {
	next if ($ent->refs("declare, define"));
	print $ent->longname() . " referenced by\n";
	foreach my $ref ($ent->ents("includeby, useby")) {
		print "\t" . $ref->longname() . "\n";
	}
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
		die usage("Error, database not specified\n") unless ($dbPath);
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
