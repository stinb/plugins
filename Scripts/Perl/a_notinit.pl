#
# Synopsis: Report variables that are not initialized
#
# Categories: Project Report, Coding Standards
#
# Languages: Ada
#
# Description:
#   Report variables that are not initialized when they
#   are defined.
#
#  For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl/
#

sub usage($) {
    print shift,"\n";
    print "Usage: a_notinit.pl\n";
    print "  -db database   Specify database only if run from uperl\n";
    print "  -def           Optional, specify to have declaration location\n";
    print "                 displayed with each variable.\n";
}


use Understand;
use Getopt::Long;
use strict;


# get options
my ($argDb,$argDef,$argHelp);
GetOptions
    (
     "db=s" => \$argDb,
     "def"  => \$argDef,
     "help" => \$argHelp,
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
die print "$language is currently unsupported" if ($language !~ /ada/i);


# get list of all objects
my @ents;
foreach my $ent ($db->ents("ada object ~loop")) {
    next if ($ent->library() =~ /standard/i);
    next if (CheckInit($ent));
    push @ents,$ent;
}


# display sorted list
foreach my $ent (sort {$a->longname() cmp $b->longname()} @ents) {
    print " ". $ent->longname();
    ShowDef($ent) if $argDef;
    print "\n";
}


# check if variable is initialized
sub CheckInit {
    my $ent = shift;

    # check for simple init ref
    return 1 if $ent->ref("ada init");

    # check for record type with init components
    my $ref = $ent->ref("ada typed","ada type record");
    if ($ref) {
	foreach my $component ($ref->ent->refs("ada declare")) {
	    return 0 unless CheckInit($component->ent());
	}
	return 1;
    }

    # check for instantiated objects
    $ref = $ent->ref("ada instanceof copy");
    return CheckInit($ref->ent()) if $ref;

    return 0;
}


# show declaration location
sub ShowDef {
    my $ent = shift;
    return unless defined ($ent);
    
    # language-specific
    my $decl;
    if ($language =~ /ada/i) {
	my @declOrder =
	    ("declarein ~spec ~body ~instance ~formal ~incomplete ~private ~stub",
	     "spec declarein",
	     "body declarein",
	     "instance declarein",
	     "formal declarein",
	     "incomplete declarein",
	     "private declarein",
	     "stub declarein");
	foreach my $type (@declOrder) {
	    last if (($decl) = $ent->refs($type,"",1));
	}
    }
    elsif ($language =~ /fortran/i) {
	($decl) = $ent->refs("definein","",1);
	($decl) = $ent->refs("declarein","",1) unless ($decl);
    }
    elsif ($language =~ /java/i) {
	($decl) = $ent->refs("definein","",1);
	($decl) = $ent->refs("declarein","",1) unless ($decl);
    }
    else { # C/C++
	($decl) = $ent->refs("definein","",1);
	($decl) = $ent->refs("declarein","",1) unless ($decl);
    }

    if ($decl) {
	print "  [File: ". $decl->file()->longname(). " ".
	"Line: ". $decl->line(),"]";
    }
}
