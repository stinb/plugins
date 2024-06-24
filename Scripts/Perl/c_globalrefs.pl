#
# Synopsis: Display all refs to globals in the current file (in the gui), that
#			are from outside the file.
#
# Categories: Project Report
#
# Languages: C
#
# Usage:
sub usage($) 
{
    return << "END_USAGE";
${ \(shift @_ ) }
Usage: globalrefs.pl
END_USAGE
}
# Currently valid for C++ only.
#

use Understand;
use Getopt::Long;
use strict;

my $help;
GetOptions(
	   "help" => \$help,
	   );

die "Must be run from within Understand" unless Understand::Gui::active();

my $db = Understand::Gui::db();
die "Must be run with a db open" if !$db;

my $file = Understand::Gui::file();
die "Must be run with an open source file" if !$file;

# find ents that have a definein ref in the current file
my @ents = ();
foreach my $ent ($db->ents("global object ~static, function ~static ~member, function member")) {
    my ($ref) = $ent->refs("definein","",1);
    if ($ref && $ref->file == $file) {
        push @ents, $ent;
    }
}

print $file->name,":\n";
foreach my $ent (@ents) {
    print " ",$ent->name(),":\n";
    foreach my $ref ($ent->refs("~declarein ~typed")) {
	if ($ref->file != $file) {
	    print "   ",$ref->kindname()," ",$ref->ent->name()," ",
	    "[file: ",$ref->file->longname()," line: ",$ref->line(),"]\n";
	}
    }
}

