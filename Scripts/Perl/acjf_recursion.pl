#
# Synopsis: Finds recursive subprograms
#
# Categories: Coding Standards, Project Report
#
# Languages: Ada, Fortran, C, Java
#
# Usage: Usage: recursion -db database

sub usage($) 
{
    return <<"END_USAGE";
${ \(shift @_) }
Usage: recursion -db database
 -db database      Specify Understand database (required for
                   uperl, inherited from Understand).
 -def              (optional) Specify to have declaration location
                   displayed with each subprogram.
END_USAGE
}

use Understand;
use Getopt::Long;
use FileHandle;
use strict;

my ($dbPath,$showDef,$help);
GetOptions( 
    "db=s" => \$dbPath,
    "def"  => \$showDef,
    "help" => \$help,
    );

# help message
die usage("") if ($help);

# open the database 
my $db=openDatabase($dbPath);

# check language
my $language = $db->language();
if ($language !~ /ada|c|fortran|java/i) {
    closeDatabase($db);
    die "$language is currently unsupported";
}

# get list of recursive subprograms
my %called = ();
my @funcs = ();
foreach my $func ($db->ents()) {
    %called = ();   # reset hash of called subprograms
    if (test_recursive($func,$func)) {
    push @funcs, $func;
    }
}

# print list of subprograms
foreach my $func (sort {lc($a->longname()) cmp lc($b->longname());} @funcs)
{
    print $func->longname(), " ";
    if ($showDef) {
	my $decl = getDeclRef($func);
	if ($decl) {
	    print "  [File: ", $decl->file()->longname(), " ",
	    "Line: ", $decl->line(),"]";
	}
    }
    print "\n";
}

closeDatabase($db);


sub test_recursive {
    my $main_func = shift;  # function we're looking for
    my $call_func = shift;  # function we're adding to the call list
    my $decl;

    # check if called func is the main func and this is not the first test
    if ($main_func == $call_func && $called{$main_func->id()}) {
	return 1;
    }

    # check if called function has already been tested
    if ($called{$call_func->id()}) {
	return 0;
    }

    # add called function to the list
    $called{$call_func->id()} = 1;

    # test each called function that has not already been tested
    foreach my $decl ($call_func->refs("call ~inactive ~ptr")) {
	if (test_recursive($main_func,$decl->ent())) {
	    return 1;
	}
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


sub openDatabase($) {
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

sub closeDatabase($) {
    my ($db)=@_;
    $db->close() if ($dbPath);
}
