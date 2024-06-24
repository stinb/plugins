# a_derived_types.pl - sample Understand perl API program
#
# Synopsis: Report all derived types and uses for a type
#
# Categories: Project Report
#
# Languages: Ada
#
# Description:
#   Report all types that are derived from a specified type.
#   Optionally, display all objects declared of these types.
#   Furthermore, optionally display all uses these objects.
#
#   For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl/
#

sub usage($) {
    print shift,"\n";
    print "Usage: a_derived_types.pl\n";
    print "  -db   database    Specify database, if used with uperl.\n";
    print "  -type typename    Specify name of type, if used with uperl -- the\n";
    print "                    current entity will be used in Understand\n";
    print "  -objs             Optional, specify to have all objects of the\n";
    print "                    type and derived types displayed.\n";
    print "  -types            Optional, specify to have all derived types displayed.\n";
    print "  -used             Optional, specify to have all object references displayed.\n";
    print "  -context [lines]  Optional, specify to have context lines before and after\n";
    print "                    the used line displayed. The default is 1 line.\n";
}


use Understand;
use Getopt::Long;
use strict;


# get options
my ($context,$dbPath,$help,@typeNames,$showObjs,$showTypes,$showUsed);
GetOptions
    (
     "context:i" => \$context,
     "db=s"      => \$dbPath,
     "help"      => \$help,
     "objs"      => \$showObjs,
     "type=s"    => \@typeNames,
     "types"     => \$showTypes,
     "used"      => \$showUsed,
     ) or die usage("");
die usage("") if ($help);
die usage("Error, type entity(s) not specified") if !@typeNames && !Understand::Gui::entity();
@typeNames = split(/,/,join(',',@typeNames));
$showUsed  = 1 if $context;
$showTypes = 1 if !$showObjs && !$showUsed;


# open the database
my $db=Understand::Gui::db();
if (!$db) {
    die usage("Error, database not specified") unless $dbPath;
    my $status;
    ($db,$status)=Understand::open($dbPath);
    die print "Error opening database: $status\n" if $status;
}


# check language
my $language = $db->language();
die print "$language is currently unsupported" if ($language !~ /ada/i);
 

# lookup specified type(s)
my @pending;
if (@typeNames) {
    @pending = map {$db->lookup($_)} @typeNames;
} else {
    push @pending,Understand::Gui::entity() if Understand::Gui::entity->kind->check("type");
}
die print "Error: no matching type entities" if (!@pending);


# lookup all derived/sub types
my @types;
while (my $pend = shift @pending) {
    push @types,$pend;
    foreach my $ref ($pend->refs("derive,typedby","type")) {
	push @pending,$ref->ent();
    }
}


# show types
if ($showTypes) {
    print "Types:\n";
    foreach my $type (sort {$a->longname() cmp $b->longname()} @types) {
	print "  ".$type->longname()."\n";
    }
    print "\n";
}


# get list of objects
my @objects;
foreach my $type (@types) {
    foreach my $ref ($type->refs("typedby","object")) {
	push @objects,$ref->ent();
    }
}


# show objects
print "Objects:\n" if $showObjs;
foreach my $obj (sort {$a->longname() cmp $b->longname()} @objects) {
    print "  ".$obj->longname()."\n" if $showObjs;
    printUsed($obj) if $showUsed;
}
print "\n" if $showObjs;



sub printUsed {
    my $obj = shift;
    my ($lexer,$lexer_filename);
    foreach my $ref ($obj->refs("useby,setby")) {
	my $line = $ref->line();
	my $filename = $ref->file->longname();
	print "    " if $showObjs;
	print $ref->kindname;
	print "  ".$obj->name() if !$showObjs;
	print "  [File: ".$filename." Line: ".$line."]\n";
	if ($context) {
	    print "      ";
	    if ($lexer_filename cmp $filename) {
		$lexer_filename = $filename;
		$lexer = $ref->file->lexer();
	    }
	    foreach my $lexeme ($lexer->lexemes($line-$context,$line+$context)) {
		print $lexeme->text();
		print "      " if $lexeme->token() =~ "Newline";
	    }
	    print "\n";
	}
    }
}
