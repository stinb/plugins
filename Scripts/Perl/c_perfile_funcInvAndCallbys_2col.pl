#
# Synopsis: Report first level of function calls and callbys, per file
#
# Categories: Project Report
#
# Languages: C
#
# Description:
#   Report invocations all functions defined in a specified file or
#   files. If no files are specified, all parsed files will be chosen.
#   Output is designed for direct import into spreadsheet as a , delimited file.
#
#   For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl/
#

sub usage($) {
    print shift,"\n";
    print "Usage: c_perfile_funcInvAndCallbys -db database\n";
    print "  -db   database    Specify Understand database (required for\n";
    print "                    uperl, inherited from Understand)\n";
    print "  -file file(s)     Specify file or files to report on\n";
    print "  -hide_unresolved  Do not display invocations to unresolved functions\n";
}

use Understand;
use Getopt::Long;
use strict;


# get options
my ($argDb,$argHelp,@argFiles,$argHideUnresolved);
GetOptions
    ("db=s"            => \$argDb,
     "help"            => \$argHelp,
     "file=s"          => \@argFiles,
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
    @files = map {$db->lookup($_,"code file ~unresolved ~unknown")} @argFiles;
    die "Error: unable to locate specified files" if (!@files);
} else {
    @files = $db->ents("code file ~unresolved ~unknown");
}

#print comma delimited headers

print ",Calls,Callby\n";


# loop through files
foreach my $file (@files)
{
    print "File:". $file->longname(). "\n";

    # loop through all refs in file
    foreach my $ent (sort {lc($a->longname()) cmp lc($b->longname())}
		     map {$_->ent()}
		     $file->filerefs
		     ("c define",
		      "c function")) 
   {
	my $first = 1;

	my $unique = 1; # only do call or callby of a given function once.
	
	# get invocations
	my $refkinds = "c call ~inactive";
	my $entkinds = $argHideUnresolved?
	    "c function ~unknown ~unresolved":
	    "c function";

	 my @calls;
	 foreach my $xref ($ent->refs($refkinds,$entkinds,$unique))
	 {
	      push(@calls,$xref->ent->name());
	 }

	# get callbys
	my $refkinds = "c callby ~inactive";
	my $entkinds = $argHideUnresolved?
	    "c function ~unknown ~unresolved":
	    "c function";

	 my @callbys;
	 foreach my $xref ($ent->refs($refkinds,$entkinds,$unique))
	 {
	      push(@callbys,$xref->ent->name());
	 }
	
	my $callToPrint;
	my $callbyToPrint;

	#loop through the calls, printing them and one from the callbyList

	print $ent->longname()."\n";	
	while ($callToPrint = pop(@calls))
	{
	  print "," . $callToPrint;
	  if ($callbyToPrint=pop(@callbys))
	  {
	   print "," . $callbyToPrint;
	  }
	  print "\n";
	}
	
	#now print rest of callby list if it was bigger than the calls one.

	while ($callbyToPrint = pop(@callbys))
	{
	  print ",," . $callbyToPrint . "\n";
	}

    }

}

