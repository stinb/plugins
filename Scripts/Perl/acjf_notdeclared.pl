#
# Synopsis: Reports unresolved items.
#
# Categories: Coding Standards, Project Report
#
# Languages: Ada, Fortran, C, Java
#
# Description:
#    Lists all entities used but not declared.
#  
#
#  For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl/
#

sub usage ($) {
    print shift,"\n";
    print "Usage: notdeclared.pl -db database [-kind kind]\n";
    print "  -db database   An Understand database.\n";
    print "  -kind kind     Optional, show only references to the specified kind(s)\n";
}


use Understand;
use Getopt::Long;
use strict;


# get options
my ($db_path,$help,@kinds,$show_used);
GetOptions
    (
     "db:s"         => \$db_path,
     "help"         => \$help,
     "kind|kinds:s" => \@kinds,
     "used"         => \$show_used,
     ) or die usage("");
die usage("") if $help;
if (@kinds) {
    @kinds = split /\s*,\s*/, lc (join ",", @kinds);
} else {
    @kinds = ("");
}

# open the database 
my $db=Understand::Gui::db();
if (!$db) {
    die usage("Error, database not specified") unless $db_path;
    my $status;
    ($db,$status)=Understand::open($db_path);
    die "Error opening database: $status\n" if $status;
}

# check language
my $language = $db->language();
die "$language is currently unsupported" if ($language !~ /ada|c|fortran|java/i);

foreach my $kind (@kinds) {
    # make kind text appropriate
    if ($kind =~ /file/i) {
	$kind = $kind . " unknown";
    } elsif (!$kind) {
	$kind = "unresolved ~file ~intrinsic, unknown file";
    } else {
	$kind = $kind . " unresolved ~intrinsic";
    }

    my @entities = $db->ents($kind);
    foreach my $ent (sort {$a->longname() cmp $b->longname()} @entities) {
        print $ent->longname();
	if ($show_used) {
	    my $ref = $ent->refs();
	    print "  [File: ".$ref->file()->longname()." Line: ".$ref->line()."]" if $ref;
	}
	print "\n";
    }
}
