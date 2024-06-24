#
# Sample Understand perl API program 
#
# Synopsis: Prints a listing of multiply-defined macros.
#
# Categories: Project Report, Coding Standards
#
# Languages: C
#
# Usage:
sub usage($) 
{
    return  <<"END_USAGE";
${ \( shift @_ ) }
Usage: find_macros.pl  -db database
    -db database      An Understand database for a C/C++ project.
END_USAGE
}

# Examples:
#
# Description:
#
#  For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl/
#  Refer to the documenation for details on using the perl API 
#  with Understand and for details on all Understand perl API calls.
# 
#  10/03/01 - Evan Knop
#
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
my $db=openDatabase($dbPath);

my $language = $db->language();

die usage("script only works on C projects.\n") if ($language !~ /C/);

my %macrohash = ();
# put ents into a hash by name...
foreach my $macroent ($db->ents("Macro ~inactive")) {
    push @{$macrohash{$macroent->longname()}}, $macroent;
}

foreach my $macroname (keys %macrohash) {
    next unless (scalar(@{$macrohash{$macroname}}) > 1);
    print "Macro ", $macroname, " multiply defined:\n";
    my @refs = map {$_->refs("Definein", "file", 0)} @{$macrohash{$macroname}};
    if ( (scalar @refs) > 1) {
        foreach my $ref (@refs) {
	    my ($file, $line) = ($ref->file()->longname(), $ref->line());
	    print "  $file: $line\n";
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

