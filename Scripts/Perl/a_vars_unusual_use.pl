#
# Synopsis: Report unusual set/use patterns for variables.
#
# Categories: Project Report, Coding Standards
#
# Languages: Ada
#
# Description:
#   Report unusual set and use patterns for variables. These
#   include set-but-never-used and used-but-never-set.
#
#   For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl/
#

sub usage($) {
    print shift,"\n";
    print "Usage: vars_unusual_use.pl\n";
    print "  -db database   Specify Understand database when\n";
    print "                 when used with uperl.\n";
    print "  -def           Optional, specify to have declaration location\n";
    print "                 displayed with each variable.\n";
    print "  -ignore        list of entities, using wildcards, to ignore.\n"
}

use Understand;
use Getopt::Long;
use strict;


# get options
my ($argDb,$argHelp,$argDef,@argIgnore);
GetOptions
    ("db=s"     => \$argDb,
     "def"      => \$argDef,
     "help"     => \$argHelp,
     "ignore=s" => \@argIgnore,
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
die "$language is currently unsupported" if ($language !~/ada/i);

# build ignores list
my @ignores;
if (@argIgnore) {
    foreach my $name (@argIgnore) {
	# quote special characters
	$name =~ s/\\/\\\\/g;
	$name =~ s/\./\\\./g;
	$name =~ s/\+/\\\+/g;
	$name =~ s/\[/\\\[/g;
	$name =~ s/\]/\\\]/g;
	# check for word boundaries
	if ($name !~ /^\^/ ) {$name = "\^".$name;}
	if ($name !~ /\$$/ ) {$name = $name."\$";}
	# replace wildcards (*/?) with regex wildcards
	$name =~ s/\*/\.\*/g;
	$name =~ s/\?/\./g;
	push @ignores,$name;
    }
}

# get variables
my @vars;
foreach my $ent ($db->ents("ada object ~task ~protected")) {
    next if ($ent->library() =~ /standard/i);
    next if $ent->type() =~ /\*|\[/;	# ignore C++ ptr vars
    next if CheckIgnore($ent);
    push @vars,$ent;
}

# build lists of unusual uses
my @no_use=();
my @no_set=();
foreach my $var (@vars) {
    my $set_ref = $var->refs("setby,modifyby");
    my $use_ref = $var->refs("useby,modifyby");
    push @no_use,$var if (defined $set_ref && !defined $use_ref);
    push @no_set,$var if (defined $use_ref && !defined $set_ref);
}

# print variables that are set but never used
if (@no_use) {
    print "Variables set but never used:\n";
    foreach my $var (@no_use) {
	print "  ". $var->longname();
	if ($argDef) {
	    my $decl = getDeclRef($var);
	    if ($decl) {
		print "  [File:". $decl->file()->longname();
		print " Line: ". $decl->line(). "]";
	    }
	}
	print "\n";
    }
    print "\n";
}

# print variables that are used but never set
if (@no_set) {
    print "Variables used but never set:\n";
    foreach my $var (@no_set) {
	print "  ". $var->longname();
	if ($argDef) {
	    my $decl = getDeclRef($var);
	    if ($decl) {
		print "  [File:". $decl->file()->longname();
		print " Line: ". $decl->line(). "]";
	    }
	}
	print "\n";
    }
    print "\n";
}


# return true if ent matches an ignore string
sub CheckIgnore {
    my $ent = shift;
    foreach my $ignore (@ignores) {
	return 1 if (($ent->name() =~ /$ignore/i) || ($ent->longname() =~ /$ignore/i));
    }
    return 0;
}


# return declaration ref (based on language) or 0 if unknown
sub getDeclRef {
    my ($ent) =@_;
    my $decl;

    return $decl unless defined ($ent);
    
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
    return $decl;
}
