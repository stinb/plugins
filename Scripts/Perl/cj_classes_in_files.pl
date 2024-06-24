#
# Synopsis: Report classes defined in files.
#
# Categories: Entity Report
#
# Languages: C,Java
#
# Description:
#   Report classes that are defined in files. One or
#   more files may be specified, or by default all
#   files in a database will be used. Typically, only
#   header files will have class definitions, although
#   it is certainly possible to have code files also
#   contain class definitions.
#
#   For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl/
#

sub usage($) {
    print shift,"\n";
    print "Usage: classes_in_files.pl -db database\n";
    print "  -db database    Specify Understand database (required for\n";
    print "                  uperl, inherited from Understand)\n";
    print "  -file file(s)   One or more files to report on\n";
    print "  -oometrics      display oo metrics for each class\n";
}

use Understand;
use Getopt::Long;
use strict;


# get options
my ($dbPath,@files,$help,$oometrics);
GetOptions
    (
     "db=s"      => \$dbPath,
     "file=s"    => \@files,
     "help"      => \$help,
     "oometrics" => \$oometrics,
     ) or die usage("");

die usage("") if ($help);

# open the database 
my $db=Understand::Gui::db();
if (!$db) {
    die usage("Error, database not specified") unless $dbPath;
    my $status;
    ($db,$status)=Understand::open($dbPath);
    die "Error opening database: $status\n" if $status;
}

# check language
my $language = $db->language();
die "$language is currently unsupported" if ($language !~ /c/i);

# lookup files
if (!@files) {
    @files = $db->ents("file ~unknown ~unresolved");
} else {
    @files = map {$db->lookup($_)} @files;
}

foreach my $file (@files) {
    my $first = 1;
    foreach my $ref ($file->refs("define","class")) {
	if ($first) {
	    $first = 0;
	    print "\n" . $file->longname . ":\n";
	}
	do_class($ref->ent());
    }
}


sub do_class {
    my $ent = shift;
    print "   Class: " . $ent->longname() . "\n";
    if ($oometrics) {
	do_metric($ent,"CountClassCoupled");
	do_metric($ent,"CountClassDerived");
	do_metric($ent,"CountDeclInstanceMethod");
	do_metric($ent,"CountDeclInstanceVariable");
	do_metric($ent,"CountDeclMethod");
	do_metric($ent,"CountDeclMethodAll");
	do_metric($ent,"PercentLackOfCohesion");
	do_metric($ent,"MaxInheritanceTree");
    }
    foreach my $ref ($ent->refs("define","class")) {
	do_class($ref->ent());
    }
}


sub do_metric {
    my ($ent,$metric) = @_;
    printf "      %-25s = %d\n",$metric,$ent->metric($metric);
}
