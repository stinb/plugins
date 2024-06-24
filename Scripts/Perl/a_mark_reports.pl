# Sample Understand PERL API Script
#
# Synopsis: Generate Ada mark reports
#
# Categories: Project Report
#
# Languages: Ada
#
# Usage:
sub usage ($)
{
    return $_[0] . << "END_USAGE";
Usage: $0 -db database [-conf file] [-mark PUNCTTABLE] [-line] [-pseudo]
       -db database      Specify Understand database (required for 
                         uperl, inherited from Understand)
       -conf file	 Specify an input file mapping punctuation to reports
			 (similar to the Adadl mark reports feature)
			 The format for this file is:
			 PUNCT TABLE
       -mark PUNCTTABLE  Specify on the command line a punctuation element 
			 and the allociated table (For example '\@date').
       -line		 Display the entire line containing the mark.
       -pseudo		 Display pseudocode associated with entities.
END_USAGE
}
#
#  For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl.html
# 
# 10 Dec 2001 - Evan Knop

use Understand;
use Getopt::Long;
use strict;

my $dbPath;
my ($conf_file, @command_marks);
my ($pseudo, $printline);
my $help;
GetOptions(
	   "db=s" => \$dbPath,
	   "c|conf=s" => \$conf_file,
	   "m|mark=s" => \@command_marks,
	   "p|pseudo" => \$pseudo,
	   "l|line" => \$printline,
	   "help" => \$help,
          );

# help message
die usage("") if ($help);

my %command_marks = map {(substr($_, 0, 1), substr($_, 1) )} @command_marks;

if ($conf_file and -r $conf_file) {
    open MARKS, $conf_file;
    foreach my $line (<MARKS>) {
	$line =~ m/\s*(.)\s*(.+)/;
	$command_marks{$1} = $2;
    }
    close MARKS;
}

die usage("No marks specified.\n") unless ( %command_marks or $pseudo);

# open the database 
my $db=openDatabase($dbPath);


# check language
if ( $db->language() !~ "Ada" ) {
    die "This script is designed for Ada\n";
}

$db->docformat('', '(--\*~[^\n]*&management~\n?)|(--\|~[^\n]*&pseudo~\n?)', '');

my @ents = $db->ents("ada ~local");

my $pattern = '[' . join( '', keys %command_marks) . ']';

foreach my $ent (@ents) {
    my $doc = $ent->doc("default", "raw");
    my @management = $doc->tag("management");
    my @pseudo = $doc->tag("pseudo");
    
    next unless ($doc->tags());
    print $ent->longname(), "\n";
    print join ("\n", @pseudo), "\n" if ($pseudo);

    if (%command_marks) {
        foreach my $line (@management, @pseudo) {
	    chomp $line;
	    my @items = ( $line =~ m/(\S*$pattern\S*)/og );
	    foreach my $element (@items) {
		my ($tag) = ( $element =~ m/($pattern)/og );
		$element = $line if ($printline);
		print ($command_marks{$tag}, ": ", $element, "\n") if $tag;
	    }
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

