#
# Sample Understand perl API program 
#
# Synopsis:  Reports filename, CountStmt and the MaxCyclomatic of units in the file.
#
# Categories: Entity Metrics
#
# Languages: Ada
#
# Usage: Designed to be used from the GUI of Understand only.

use Understand;
use strict;

my $dbPath;
my $help;

 # open the database 
my $db=Understand::Gui::db();
if (!$db) {
    die usage("Error, database not specified") unless $dbPath;
    my $status;
    ($db,$status)=Understand::open($dbPath);
    die "Error opening database: $status\n" if $status;
  }

my $AdaFile;

foreach $AdaFile (sort {lc($a->longname()) cmp lc($b->longname());} $db->ents("file"))
{
        next if ($AdaFile->library() eq "Standard");
        next if (!  $AdaFile->filerefs("declarein"));
	
	print $AdaFile->name() . "," . $AdaFile->metric("CountStmtExe");
	
	#now calculate maximum complexity of nested items in the file
	#do it by sorting them and printing the top dog
		
	my @progUnits = reverse sort {$b->ent()->metric("CyclomaticModified") <=> $a->ent()->metric("CyclomaticModified")} $AdaFile->filerefs("declarein");
	print "," . pop(@progUnits)->ent()->metric("CyclomaticModified");
	print ("\n");

}

