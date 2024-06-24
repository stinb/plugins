# c_funcscalledbyptr.pl
#
# Synopsis: Report functions that have their address taken (used).
#
# Categories: Project Report
#
# Languages: C
#

use Understand;
use Getopt::Long;
use strict;

my $dbPath;
my $configFile;
my $excludeFile;
my $verbose;

# grab command-line parameters
GetOptions("db=s" => \$dbPath);

my $db;
my $status;

# open Understand project database
$dbPath ? ($db,$status) = Understand::open($dbPath) : $db = Understand::Gui::db();
die("Understand project database open error: $status\n")
	if($status);

# get a list of each file in the project
foreach my $fileEnt ($db->ents("file ~unknown ~unresolved")) {
    my $newFile = 1;
    foreach my $funcRef ($fileEnt->filerefs("define","function")) {
        my @entRefs = $funcRef->ent()->refs("useby ptr");
        if(@entRefs) {
            if($newFile) {
                $newFile = 0;
                print($fileEnt->name()."\n");
            } # end if($newFile)
            
            print("\t".$funcRef->ent()->name()." - ");
            foreach my $entRef (@entRefs) {
                print($entRef->file()->name()." (".$entRef->line()."/".$entRef->column().")\n");
            } # end foreach my $entRef (@entRefs)
        } # end if(@entRefs)
    } # end foreach my $funcRef ($fileEnt->refs("define","function"))
} # end foreach my $fileEnt ($db->ents("file ~unknown ~unresolved"))

# close the open Understand database
$db->close() if($dbPath);
