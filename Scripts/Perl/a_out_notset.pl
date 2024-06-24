#
# Synopsis: Report "in out" and "out" parameters that are not set
#
# Categories: Project Report, Coding Standards
#
# Languages: Ada
#
# Description:
#   Report "in out" and "out" parameters that are not set.
#
#  For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl/
#

sub usage($) {
    print shift,"\n";
    print "Usage: a_notset.pl\n";
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


# get list of all "out" and "in out" parameters that are not set
my @ents;
foreach my $ent ($db->ents("ada parameter")) {
    next if ($ent->library() =~ /standard/i);
    next if (! $ent->ref("ada declarein body"));
    next if (CheckIn($ent));
    next if (CheckSet($ent));
    push @ents,$ent;
}


# display sorted list
foreach my $ent (sort {$a->longname() cmp $b->longname()} @ents) {
    print " ". $ent->longname();
    ShowDef($ent) if $argDef;
    print "\n";
}

# check if parameter is a plain "in" parameter (not "out" or "in out")
sub CheckIn {
    my $ent = shift;

    my $typetext = $ent->type();
    # print $ent->longname() . " " . $typetext . "\n";
    
    return 0 if ($typetext =~ /^out\s/);
    return 0 if ($typetext =~ /^in\sout/);
    
    return 1;
}

sub CheckSet {
    my $ent = shift;
    return 1 if $ent->ref("ada setby");
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
