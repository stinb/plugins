#
# Sample Understand PERL API Script
#
# Synopsis: Lists global object calls, sets, and usage by function
#
#   Lists all functions that reference global objects
#   and the global objects that they use.
#
#   Requires an existing Understand database
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
Usage: globals_used.pl -db database
  -db database    Specify Understand database (required for
	          uperl, inherited from Understand)
END_USAGE
}

#  For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl.html
# 
#  4-Aug-2001 DLL - updated for Understand::Gui::db()
# 10-Sep-2001 DLL - add support for all languages


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

# check language
my $language = $db->language();

my $kinds = "subroutine ~unresolved ~unknown,procedure,function ~unresolved ~unknown";

sub icmpln($$) {
    return (lc($_[0]->longname()) cmp lc($_[1]->longname()));
}

# search each of the functions for globals usage.
foreach my $func (sort {icmpln $a, $b} $db->ents($kinds)) {
    # get the globals that are set, used or called.
    my @useEnts = $func->refs("use, set, call",
                              "object ~local,parameter,Block Variable");
    
    my %ent_cache = ();
    if ( @useEnts ) {
	print "func: " . $func->longname(),":\n";
	foreach my $used (sort {icmpln $a->ent(), $b->ent()} @useEnts) {
	    next if ($ent_cache{$used->ent()->id()}{$used->kindname()}++);
	    print "  global ", $used->kindname(), ": ", 
		$used->ent()->longname(), 
		"  (", $used->ent()->kindname(),")\n";
	}
	print "\n";
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

