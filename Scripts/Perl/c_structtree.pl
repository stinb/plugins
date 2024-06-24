#
# Synopsis: Reports struct dependencies for specified struct entities
#
# Categories: Entity Report
#
# Languages: C
#
# Usage:

sub usage($) 
{
    return <<"END_USAGE";
${ \( shift @_ ) }
Usage: structtree.pl -db database [-ent entity]
 -db database         Specify Understand database (required for 
                      uperl, inherited from Understand)
 -ent entity          (optional) An entity to report on.
 -list                (optional) List entities
END_USAGE
}

# Description:
#  Generates a structure dependency tree to stdout.  
#  Requires an existing Understand database.
#
#  For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl/
#  Refer to the documenation for details on using the perl API 
#  with Understand and for details on all Understand perl API calls.
# 
#  9 Aug 2001, DLL
#
use Understand;
use Getopt::Long;
use strict;

my $dbPath;
my $entity;
my $list;
my $help;
GetOptions("db=s" => \$dbPath,
           "entity=s" => \$entity,
	   "list" => \$list,
           "help" => \$help,
           );

# help message
die usage("") if ($help);

# arg consistancy
die "one of -list|-entity required\n" . usage("") if (!$list&&!$entity);
# open the database 
my $db=openDatabase($dbPath);

# check language
if ( $db->language() !~ "C" ) {
    die "This script is designed for C only\n";
}

my %alreadyDone = ();

my $struct_kinds = "Struct,Class,Union,Typedef";
if ($list) {
    listEntities($struct_kinds);
}
elsif ($entity) {
    
    # get matching entites
    my @ents=$db->lookup($entity,$struct_kinds);

    # report if none found
    if (!@ents) {
	print "entitiy $entity not found in $dbPath\n";
    }
    
    # graph all that were found
    foreach my $ent (@ents) {
	graphEntity($ent,"");
    }
}

closeDatabase($db);


# subroutines


sub graphEntity($$)
{
    my ($ent,$indent) = @_;

    my $name = $ent->name();
    my $typedby_refs = $ent->refs("Typedby");
    my $endby_refs = $ent->refs("Endby");
    my ($typed_ref) = $ent->refs("Typed","",1);

    # bail if already done
    if ( $alreadyDone{$ent->id()} ) {
	return;
    }
    $alreadyDone{$ent->id()} = 1;

    # display it if it has no typedby ref or has an endby ref.
    if (!$typedby_refs || $endby_refs) {
	print "$indent $name\n";
    }

    # get it's "typed" reference
    if ($typed_ref) {
	# follow it's "typed" 
	graphEntity($typed_ref->ent(),"  ".$indent);
    }
    elsif ($ent->kind()->check($struct_kinds)) {
	# graph all members 
	foreach my $ref ($ent->refs("Define")) {
	    graphEntity($ref->ent,"  ".$indent);
	}

    }
}


sub listEntities($)
{
    my ($struct_kinds) = @_;

    # show user all entites
    foreach my $ent (sort {$a->name() cmp $b->name(); } $db->ents($struct_kinds)) {
	my $where;
	my ($definein_ref) = $ent->refs("Definein","",1);
	if ($definein_ref) {
	    my $line = $definein_ref->line();
	    my $file = $definein_ref->file()->name();
	    $where = "$file\{$line\}";
	}
	my $name = $ent->name();
	my $kind = $ent->kind()->name();
	if ( $name !~ '\[unnamed\]' ) {
	    print "$name [$kind]: $where\n";
	}
    }
}


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


