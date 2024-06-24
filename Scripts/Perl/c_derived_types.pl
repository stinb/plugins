#
# Synopsis: Report all derived types and uses for a type.
#
# Categories: Project Report
#
# Languages: C
#
# Description:
#   Report all typedefs that are derived from a specified type.
#   The type may be a typedef or a built-in type, such as 'int'.
#   Optionally, display all objects declared of these types.
#   Furthermore, optionally display all uses these objects.
#
#   For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl/
#

sub usage($) {
    print shift,"\n";
    print "Usage: c_derived_types.pl\n";
    print "  -db   database    Specify database, if used with uperl.\n";
    print "  -def              Optional, display definition location for each entity.\n";
    print "  -type typename    Specify name of type.\n";
    print "  -objs             Optional, display all variables of the derived types.\n";
    print "  -types            Optional, display all typedefs of the derived types.\n";
}


use Understand;
use Getopt::Long;
use strict;


# get options
my ($argDb,$argDef,$argHelp,$argType,$argObjs,$argTypes);
GetOptions
    (
     "db=s"      => \$argDb,
     "def"       => \$argDef,
     "help"      => \$argHelp,
     "objs"      => \$argObjs,
     "type=s"    => \$argType,
     "types"     => \$argTypes,
     ) or die usage("");
die usage("") if ($argHelp);
die usage("Error, type not specified") if !$argType && !Understand::Gui::word();
$argTypes = 1 if !$argObjs;


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


# get the type name or entity
if (!$argType) {
    $argType = Understand::Gui::selection();
}

# lookup derived types
my @types;
my @pending;
my $by_name = 0;
if (my $ent=$db->lookup($argType)) { # start with typedef if possible
    push @pending,$ent;
} else {			     # start with typedefs with textual usage
    $by_name = 1;
    foreach my $ent ($db->ents("c typedef")) {
	if ($ent->type =~ /\b$argType\b/) {
	    push @pending,$ent;
	}
    }
}
while (my $pend = shift @pending) {
    push @types,$pend;
    foreach my $ent ($pend->ents("c typedby","c typedef")) {
	push @pending,$ent;
    }
}


# display derived types
if ($argTypes) {
    print "Types:\n";
    foreach my $type (sort {$a->longname() cmp $b->longname()} @types) {
	print "  ". $type->longname();
	PrintDecl($type) if $argDef;
	print "\n";
    }
    print "\n";
}


# display objects
if ($argObjs) {
    my @objects;
    # get objects that have the textual type
    if ($by_name) {
	foreach my $obj ($db->ents("c object ~member ~unknown,c parameter")) {
	    if ($obj->type =~ /\b$argType\b/) {
		push @objects,$obj;
	    }
	}
    }
    # get objects by derived type ents
    foreach my $type (@types) {
	foreach my $obj ($type->ents("c typedby","c object ~member ~unknown,c parameter")) {
	    push @objects,$obj;
	}
    }
    # display objects
    print "Objects:\n";
    foreach my $obj (sort {$a->longname() cmp $b->longname()} @objects) {
	print "  ". $obj->longname();
	PrintDecl($obj) if $argDef;
	print "\n";
    }
    print "\n";
}


# print the declaration file/line, if available
sub PrintDecl {
    my ($ent) =@_;
    my $decl;

    return unless defined ($ent);
    
    # language-specific
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
	print "  [File: ". $decl->file()->longname();
	print " Line: ". $decl->line(). "]";
    }
}
