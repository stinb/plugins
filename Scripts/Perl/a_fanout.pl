#
# Synopsis: Report FANOUT metrics for ada subprograms
#
# Categories: Project Metrics
#
# Languages: Ada
#
# Description:
#   Report FANOUT metrics for ada subprograms. A common
#   definition of the fanout metric is the number of
#   calls to external subprograms plus the number of
#   modifications of external data. Sometimes, however,
#   just the number of calls or just the number of
#   modifications can be useful.
#
#   There are different possible definitions for 'external'
#   references. An external referenced entity may simply
#   be an entity not defined in the immediate subprogram
#   scope; or may be an entity not defined in the immediate
#   package scope; or may be an entity not defined in the
#   same library unit. The default is the third case, outside
#   the current library unit scope. Specify '-external_local'
#   to indicate calculation with the first case, outside the
#   immediate subprogram scope. Specify '-external_package'
#   to indicate calculation with the second case, outside
#   the immediate package scope.
#
#   For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl/
#

sub usage($) {
    print shift,"\n";
    print "Usage: a_fanout.pl\n";
    print "  -db database       Specify the Understand database if run from uperl\n";
    print "  -csv               Output in a format suitable for spreadsheet import\n";
    print "  -csv_titles        Output in -csv mode, including column titles\n";
    print "  -external_local    Define externals as outside the immediate subprogram scope\n";
    print "  -external_package  Define externals as outside the immediate package scope\n";
    print "  -only_fanout       Display only fanout value, instead of all values\n";
    print "  -only_numcalls     Display only number of calls value, instead of all values\n";
    print "  -only_numsets      Display only number of sets value, instead of all values\n";
}

use Understand;
use Getopt::Long;
use strict;


# get options
my ($argDb,$argHelp,$argCsv,$argCsvTitles,
    $argExternalLibraryUnit,$argExternalLocal,$argExternalPackage,
    $argOnlyFanout,$argOnlyNumCalls,$argOnlyNumSets);
GetOptions
    ("db=s"		=> \$argDb,
     "help"		=> \$argHelp,
     "csv"		=> \$argCsv,
     "csv_titles"	=> \$argCsvTitles,
     "external_local"	=> \$argExternalLocal,
     "external_package"	=> \$argExternalPackage,
     "only_fanout"	=> \$argOnlyFanout,
     "only_numcalls"	=> \$argOnlyNumCalls,
     "only_numsets"	=> \$argOnlyNumSets,
     ) or die usage("");
die usage("") if ($argHelp);
$argCsv = 1 if $argCsvTitles;
if (!$argOnlyFanout && !$argOnlyNumCalls && !$argOnlyNumSets) {
    $argOnlyFanout = $argOnlyNumCalls = $argOnlyNumSets = 1;
}
$argExternalLibraryUnit = 1 if (!$argExternalPackage && !$argExternalLocal);

# open the database
my $db = Understand::Gui::db();
if (!$db) {
    die usage("Error, database not specified") unless $argDb;
    my $status;
    ($db,$status) = Understand::open($argDb);
    die "Error opening database: $status\n" if $status;
}

# check language
my $language = $db->language();
die "$language is currently unsupported" if ($language !~/ada/i);

# lookup subprograms
my @subprograms = ();
foreach my $ent ($db->ents("ada function,".
			   "ada procedure")) {
    next if $ent->library("Standard");
    next if $ent->refs("ada instanceof");
    push @subprograms, $ent;
}

# calculate values
my (%valuesFanout,%valuesNumCalls,%valuesNumSets);
foreach my $ent (@subprograms) {
    my ($numcalls,$numsets,$fanout);
    if ($argOnlyNumCalls || $argOnlyFanout) {
	$numcalls = calcNumCalls($ent);
	$valuesNumCalls{$ent->id()} = $numcalls if ($argOnlyNumCalls);
    }
    if ($argOnlyNumSets || $argOnlyFanout) {
	$numsets = calcNumSets($ent);
	$valuesNumSets{$ent->id()} = $numsets if ($argOnlyNumSets);
    }
    if ($argOnlyFanout) {
	$fanout = $numcalls + $numsets;
	$valuesFanout{$ent->id()} = $fanout;
    }
}

# output csv values
if ($argCsv) {
    if ($argCsvTitles) {
	print "Name";
	print ",Fanout" if ($argOnlyFanout);
	print ",NumCalls" if ($argOnlyNumCalls);
	print ",NumSets" if ($argOnlyNumSets);
	print "\n";
    }
    foreach my $ent (sort {lc($a->longname()) cmp lc($b->longname())} @subprograms) {
	print $ent->longname();
	print ",". $valuesFanout{$ent->id()} if ($argOnlyFanout);
	print ",". $valuesNumCalls{$ent->id()} if ($argOnlyNumCalls);
	print ",". $valuesNumSets{$ent->id()} if ($argOnlyNumSets);
	print "\n";
    }
}

# output non-csv values
else {
    foreach my $ent (sort {lc($a->longname()) cmp lc($b->longname())} @subprograms) {
	print $ent->longname(),"\n";
	print "  Fanout = ". $valuesFanout{$ent->id()}. "\n" if ($argOnlyFanout);
	print "  Num Calls = ". $valuesNumCalls{$ent->id()}. "\n" if ($argOnlyNumCalls);
	print "  Num Sets = ". $valuesNumSets{$ent->id()}. "\n" if ($argOnlyNumSets);
	print "\n";
    }
}


sub calcNumCalls {
    my $ent = shift;
    my $num = 0;
    foreach my $ref ($ent->refs("ada call ~indirect","",1)) {
	++$num if checkExternal($ent,$ref->ent());
    }
    return $num
}

sub calcNumSets {
    my $ent = shift;
    my $num = 0;
    foreach my $ref ($ent->refs("ada set","",1)) {
	++$num if (checkExternal($ent,$ref->ent()));
    }
    return $num
}

sub checkExternal {
    my $scope = shift;
    my $ent = shift;

    # get scope
    if (!$argExternalLocal) {
	while (1) {
	    last if ($argExternalPackage && $scope->kind->check("ada package"));
	    my ($parent) = $scope->ents("ada declarein ~instance ~file");
	    last if !defined $parent;
	    $scope = $parent;
	}
    }

    # check parents of ent
    while ($scope != $ent) {
	($ent) = $ent->ents("ada declarein ~instance ~file");
	return 1 if !defined $ent;
    }
    return 0;
}
