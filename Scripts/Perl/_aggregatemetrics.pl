#
# Sample Understand perl API program 
#
# Synopsis: Outputs aggregate metrics for groups of files.
#
# Languages: All
#
# Categories: Project Metrics,Entity Metrics
#
# Usage:
sub usage($) 
{
    return @_[0].  <<"END_USAGE";
Usage: $0  -db database  [-name name] [-dir dir]
  -db database      Specify Understand database (required for
                    uperl, inherited from Understand)
  -name name	    Report aggregate metrics for entities of a given name..
  -dir dir	    Only files found in dir directory are to be counted
  -kinds kinds	    Only report metrics for the specified kinds.
END_USAGE
}

# Examples:
#
# Description:
#  Outputs useful metrics for a group of files.
#  Default is for all entities that have metrics.
#  Requires an existing Understand database
#
#  For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl/
#  Refer to the documenation for details on using the perl API 
#  with Understand and for details on all Understand perl API calls.
# 
#  18/10/01  - Evan Knop
#
use Understand;
use Getopt::Long;
use strict;

my $dbPath;
my ($help);
my ($name);
my ($dir);
my (@kinds);
GetOptions(
	   "db=s" => \$dbPath,
	   "help" => \$help,
	   "name:s" => \$name,
	   "dir:s" => \$dir,
	   "kind|kinds:s", \@kinds,
           );

$name = "" unless $name;
$dir = "" unless $dir;

if (@kinds) {
    @kinds = split /\s*,\s*/, join( ",", @kinds);
} else {
    @kinds = ("");
}

# strip off trailing "/" if present...
$dir =~ s:/$::;

# help message
die usage("") if ($help);

# open the database 
my $db=openDatabase($dbPath);

my $language = $db->language();
my @entlist;

if ($name) {
    # if we have a name, we should use that.
    my @entities = grep {$_->name() =~ m:$name:} $db->ents("~unknown ~unresolved");
    # filter by directory and kind, if necessary...
    foreach my $entity (@entities) {
	next unless grep {$entity->kindname() =~ m/$_/i} @kinds;
	if (grep {$_->file()->longname() =~ m:$dir(/|\\):} $entity->refs() ) {
	    push @entlist, $entity;
	}
    }
} else {
    # do files
    # match directory names on PC or *nix
    @entlist = grep {$_->longname() =~ m:$dir(/|\\):} $db->ents("file");
}
my (%aggregate, %count);

foreach my $ent (@entlist) {
    print $ent->longname() ."\n";
    my @metrics = $ent->metrics();
    next unless @metrics;
    my @values = $ent->metric(@metrics);
    next unless ($#metrics == $#values);
    while (@metrics and @values) {
	my $metric = pop @metrics;
	my $value = pop @values;
	$count{$metric} += 1;
	if ($metric =~ m/^Max/) {
	    ($aggregate{$metric} < $value) and $aggregate{$metric} = $value;
	} elsif ($metric =~ m/^Avg/) {
	    $aggregate{$metric} += $value;
	} elsif ($metric =~ m/^Count/) {
	    $aggregate{$metric} += $value;
	} elsif ($metric =~ m/^Percent/) {
	    $aggregate{$metric} += $value;
	}
    }
}

foreach my $key (keys %aggregate) {
    next unless $count{$key};
    if ($key =~ m/^(Avg|Percent)/) {
	$aggregate{$key} = $aggregate{$key} / $count{$key};
    }
    
    print "$key: $aggregate{$key} (out of $count{$key} values)\n";
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

