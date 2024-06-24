#
# Synopsis : Lists specified references kinds of specified entity kinds
#
# Categories: Project Report
#
# Languages: All
#
# Usage:
sub usage ($)
{
    return << "END_USAGE";
${ \( shift @_ ) }
Usage: uperl listrefs.pl -db database -ents kinds -refs kinds
	    -db database     Specify Understand database (required for
	                     uperl, inherited from Understand)
	    -ents kinds      Specify entity kinds to process
	    -refs kinds      Specify reference kinds to process
END_USAGE
}

#  For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl/
#  Refer to the documenation for details on using the perl API 
#  with Understand and for details on all Understand perl API calls.
# 
#  13-Sep-2001 DLL New

use Understand;
use Getopt::Long;
use strict;

my $dbPath;
my $help;
my $entKinds;
my $refKinds;
GetOptions("db=s"   => \$dbPath,
           "ents=s" => \$entKinds,
           "refs=s" => \$refKinds,
           "help" => \$help,
	   ) || die usage(""); 

# help message
die usage("") if ($help);

# open the database 
my $db=openDatabase($dbPath);

# provide defaults if not specified 
$entKinds = "" unless defined $entKinds;
$refKinds = "" unless defined $refKinds;

print "entKinds = $entKinds\n";
print "refKinds = $refKinds\n";
print "\n";

# get all ents of specified kinds
my @sortedEnts = sort {lc($a->longname()) cmp lc($b->longname())} $db->ents($entKinds);
foreach my $ent (@sortedEnts) {

    # skip Ada standard library ents
    next if ($ent->library() eq "Standard");

    # get refs of specified kinds
    my @refs = $ent->refs($refKinds);

    # skip if none
    next if ! @refs;

    # print ent info
    print "  " . $ent->name() . " (" . $ent->kind()->name() . ")\n"; 

    # print ref info for each 
    foreach my $ref (@refs) {
	print "    ref: (" . $ref->kind()->name() . ") " . $ref->ent()->name() . "\n";
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

