#
# Synopsis: Report all defined functions and the file they are defined in.
#
# Categories: Project Report
#
# Languages: C
#
# Description:  Lists all functions that are defined and the file they are defined in.
#  
#   For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl/
#

sub usage($) {
    print shift,"\n";
    print "Usage: c_listfuncs.pl\n";
    print "  -db   database    Specify database, if used with uperl.\n";
}


use Understand;
use Getopt::Long;
use strict;


# get options
my ($argDb,$argDef,$argHelp,$argType,$argObjs,$argTypes);
GetOptions
    (
     "db=s"      => \$argDb,
     ) or die usage("");
die usage("") if ($argHelp);


# open the database
my $db=Understand::Gui::db();
if (!$db) {
    die usage("Error, database not specified") unless $argDb;
    my $status;
    ($db,$status)=Understand::open($argDb);
    die print "Error opening database: $status\n" if $status;
}


# check language
my $language = $db->language();
die print "$language is currently unsupported" if ($language !~ /c/i);


#actually do it.. first get a list of all functions. Then print those
#that have a definein reference.

my @funcs = $db->ents("Function");
foreach my $func (@funcs) {
    my $defineref = $func->ref("definein");
    print $func->longname(),", ",$defineref->file()->longname()," ,", $defineref->line(), "\n" if $defineref;
}


