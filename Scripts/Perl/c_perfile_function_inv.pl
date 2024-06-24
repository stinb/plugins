#
# Synopsis: Report function invocations, per file
#
# Categories: Project Report
#
# Languages: C
#
# Description:
#   Report invocations all functions defined in a specified file or
#   files. If no files are specified, all parsed files will be chosen.
#
#   For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl/
#

sub usage($) {
    print shift,"\n";
    print "Usage: perfile_function_inv.pl -db database\n";
    print "  -db   database    Specify Understand database (required for\n";
    print "                    uperl, inherited from Understand)\n";
    print "  -file file(s)     Specify file or files to report on\n";
    print "                    specify multiple files with multiple instances of -file\n";
    print "  -full_refs        Display file/line information\n";
    print "  -hide_unresolved  Do not display invocations to unresolved functions\n";
}

use Understand;
use Getopt::Long;
use strict;


# get options
my ($argDb,$argHelp,@argFiles,$argFullrefs,$argHideUnresolved);
GetOptions
    ("db=s"            => \$argDb,
     "help"            => \$argHelp,
     "file=s"          => \@argFiles,
     "full_refs"       => \$argFullrefs,
     "hide_unresolved" => \$argHideUnresolved,
     ) or die usage("");
die usage("") if ($argHelp);

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
die "$language is currently unsupported" if ($language !~/c/i);

# lookup files
my @files;
if (@argFiles) {
    @files = map {$db->lookup($_,"file ~unresolved ~unknown")} @argFiles;
    die "Error: unable to locate specified files" if (!@files);
} else {
    @files = $db->ents("file ~unresolved ~unknown");
}

# loop through files
foreach my $file (@files) {
    print "\nFile: ". $file->longname(). "\n";

    # loop through all refs in file
    foreach my $ent (sort {lc($a->longname()) cmp lc($b->longname())}
		     map {$_->ent()}
		     $file->filerefs
		     ("c define",
		      "c function")) {
	my $first = 1;

	# display invocations
	my $refkinds = "c call ~inactive";
	my $entkinds = $argHideUnresolved?
	    "c function ~unknown ~unresolved":
	    "c function";
	my $unique = !$argFullrefs;
	
	foreach my $xref ($ent->refs($refkinds,$entkinds,$unique)) {
	    if ($first) {
		$first = 0;
		print "  ". $ent->longname(). ":\n";
	    }
	    print "    ". $xref->ent->name();
	    if ($argFullrefs) {
		print "  [File: ". $xref->file()->longname().
		      " Line: ". $xref->line().
		      "]";
	    }
	    print "\n";
	}
    }
}
