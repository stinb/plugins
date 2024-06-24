#
# Sample Understand PERL API Script
#
# Synopsis: A tool to display formatted javadoc comments for the current entity
#
# Categories: Entity Report
#
# Languages: Java
#
# Usage:
sub usage ($)
{
    return $_[0] . << "END_USAGE";
Run this tool from inside understand to view javadoc comments for the current entity.
END_USAGE
}

#  For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl.html
# 
# 28-Feb-2002  - Evan Knop
#


use Understand;
use Getopt::Long;
use strict;

my $doxml=0;
my $help;
GetOptions(
	   "x|xml" => \$doxml,
	   "help" => \$help,
          );

# help message
die usage("") if ($help);

# open the database 
my $db=Understand::Gui::db();

die usage("This script must be run from the Understand GUI.") unless $db;

# check language
my $language = $db->language();
print "Database does not contain java\n" if ($language !~ m/Java/i);

$db->docformat('\/\*{2}',
	       '(?xs)		# use extended regexes.
		(?<!\n)		# anchor at the beginning of the tag.
		\s*\*\s+\@	# " * @" begins a javadoc comment
		~\w+&@~		# match the first word as the tag
		\s+		# eat up whitespace
		~.+?&@~		# match (minimally) the rest of the comment.
		(?=		# until (zero-width assertion)
		(\n\s*\*\s+\@)	# a new pattern would match
		| (\n\s*\*+\/)	# or end-of-comment
		| $		# or the end of the pattern.
		)',
	       '\*\/');

my $ent = Understand::Gui::entity();

die "Could not find entity\n" unless ($ent);

if ($ent->kind()->check("package")) {
    print bold("Package", $ent->longname()), "\n";
    my $doc = $ent->doc();
    print "\t", shortdesc($doc), "\n";
    
    print bold("Interface Summary"), "\n";
    foreach my $interface ($ent->ents("", "interfaces")) {
	print $interface->name(), "\t- ", shortdesc($interface->doc()), "\n";
    }

    print bold("Class Summary"), "\n";
    foreach my $class ($ent->ents("", "classes")) {
	print $class->name(), "\t- ", shortdesc($class->doc()), "\n";
    }
    
    print bold ("Package", $ent->longname(), "Description"), "\n";
    print $doc->body(), "\n";

    if ($doc->tag("since")) {
	print bold("Since:"), " ", join(" ", $doc->tag("since")), "\n";
    }

}

if ($ent->kind()->check("class, interface")) {
    print bold($ent->kind()->name(), $ent->name()), "\n";
    my $doc = $ent->doc();
    my @parents = ($ent);
    while (getparent($parents[0])) {
	unshift @parents, getparent($parents[0]);
    }
    print scalar(@parents), ": ", map {($_)? $_->name(): "____"} @parents;
    my $indent = 0;
    while (@parents) {
	my $elem = shift @parents;
	if ($indent > 0) {
	    print ("    " x $indent, " |  ", "\n");
	    print ("    " x $indent, " +--", $elem->longname(), "\n");
	} else {
	    print ($elem->longname(), "\n");
	}
	$indent++;
    }
    
    print bold("Implemented Interfaces:"), "\n";
    print join(", ", map {$_->name()} $ent->ents("", "interface")), "\n";
    
    print bold("Direct Subclasses:"), "\n";
    print join(", ", map {$_->name()} $ent->ents("derive")), "\n\n";
    
    print (($ent->kind()->check("public"))? "public":"private"), 
	  $ent->kind()->name(), $ent->name(), "\n";
    print "extends ", getparent($ent)->name(), "\n";

    print $doc->body(), "\n";
    
    if ($doc->tag("since")) {
	print bold("Since:"), " ", join(" ", $doc->tag("since")), "\n";
    }
    
    my (@constructors, @methods);
    foreach my $func ($ent->ents("", "function")) {
	if ($func->name() eq $ent->name()) {
	    push @constructors, $func;
	} else {
	    push @methods, $func;
	}
    }
    # a bunch of summaries I don't care about right now...
    
    print bold("Constructor Detail"), "\n";
    print bold($ent->name()), "\n"; 
    foreach my $cons (@constructors) {
	reportfunc($cons);
    }
    
    print "\n", bold("Method Detail"), "\n";
    foreach my $method (@methods) {
	reportfunc($method);
    }
    
}
    
if ($ent->kind()->check("function")) {
    reportfunc($ent);
}

if ($ent->kind()->check("object")) {
    print $ent->kind()->name(), "\n";
}

# subroutines

sub bold(@) {
    if ($doxml) {
	return join " ", "<bold>", @_, "</bold>";
    } else {
	return join " ", "*", @_, "*";
    }
}

sub typedname($) {
    if ($_[0]->type()) {
	return $_[0]->type() . " " . $_[0]->name();
    } else {
	return $_[0]->name();
    }
}

sub list($@) {
    my $indent = shift @_;
    if ($doxml) {

    } else {
	return join("", map { ("\n", $indent, "- ", $_) } (@_));
    }
}

sub shortdesc($) {
    return ($_[0]->body() =~ m/^(.+?)\n/);
}
    

sub reportfunc($) {
    my $func = $_[0];
    my $doc = $func->doc("both");
    my $line = "";

    print bold($func->name()), "\n";
    $line = "public " if ($func->kind()->check("public"));
    $line .= ($func->type()? $func->type() : "void");
    $line .= " " . bold($func->name()) . " (";
    # this will print all the params, separated by ", "
    print $line, join(", ", map {typedname $_} ($func->ents("", "parameter"))), ")\n\t", $doc->body(),"\n";
    if ($doc->tag("param")) {
	print bold("Parameters:"), list( "  ", $doc->tag("param")), "\n";
    }
    if ($doc->tag("return")) {
	print bold("Returns:"), list("  ",$doc->tag("return")), "\n";
    }
}

sub getparent($) {
    my $ent = $_[0];
    return ($ent->ents("Extend", "class, interface"))[0];
}
