#
# Synopsis: Report function cross references, per file
#
# Categories: Project Report
#
# Languages: C
#
# Description:
#   Report cross reference information for all functions
#   defined in a specified file or files. If no files are
#   specified, all parsed files will be chosen.
#
#   For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl/
#

sub usage($) {
    print shift,"\n";
    print "Usage: perfile_function_xref.pl -db database\n";
    print "  -db   database   Specify Understand database (required for\n";
    print "                   uperl, inherited from Understand)\n";
    print "  -file file(s)    Specify file or files to report on\n";
}

use Understand;
use Getopt::Long;
use strict;


# get options
my ($argDb,$argHelp,@argFiles);
GetOptions
    ("db=s"   => \$argDb,
     "help"   => \$argHelp,
     "file=s" => \@argFiles,
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
	print "  ". $ent->longname(). ":\n";

	# display xrefs
	foreach my $xref ($ent->refs
			  ("c derive,c callby ~inactive,c declarein,c definein,".
			   "c friendby,c includeby ~inactive,c modifyby,".
			   "c setby,c typeby,c useby ~inactive")) {
	    print "    ". $xref->kindname().
		" ". $xref->ent->name().
		"  [File: ". $xref->file()->longname().
		" Line: ". $xref->line().
		"]\n";
	}
    }
}
