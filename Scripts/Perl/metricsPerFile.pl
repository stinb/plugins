
# Sample Understand PERL API Script
#
# Synopsis: Shows all metrics per file
#
# Language: All languages 
#
# Usage:
sub usage ($)
{
    return << "END_USAGE";
${ \(shift @_) }
Usage: $0 -db database
       -db database      Specify Understand database (required for
                         uperl, inherited from Understand)
END_USAGE
}
#
#  For the latest Understand perl API documentation, see
#      http://www.scitools.com/perl.html
#
#  28-Jan-2019 KJ

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

my @files = $db->ents("file ~unresolved ~unknown");

    foreach my $func (sort {lc($a->longname()) cmp lc($b->longname());} @files){
        if ($func->library() ne "Standard") {
            PrintExitData($func);
        }
    }
    

closeDatabase($db);


# subroutines
#Return the number of exits, goto statements, function calls for the entity
sub PrintExitData {
    my ($ent) = @_;					

    print $ent->name()."\n";
    foreach my $metric ($ent->metrics()){
      print "\t${metric} : ".$ent->metric($metric)."\n";
    }  
}


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
    $db->close() if (!Understand::Gui::active());
}
