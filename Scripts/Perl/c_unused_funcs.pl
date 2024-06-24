#
# Synopsis: Report unused functions.
#
# Categories: Project Report,Coding Standards
#
# Languages: C
#
# Description:
#  Reports functions that are not called or used by name (function pointer).
#

sub usage {
    print shift,"\n";
    print "Usage: unused_funcs.pl -db database\n";
    print "  -calls            Optional; specify to exclude functions that call other functions.\n";
    print "  -db databse       Specify Understand database, if run from uperl.\n";
    print "  -def              Optional; specify to have definition location shown.\n";
    print "  -exclude pattern  Optional; specify to exclude functions from being\n";
    print "                    reported. Allows simple wildcards (* and ?)\n";
    return 1;
}

my $abort=0;
sub error {
    print shift,"\n";
    $abort = 1;
    return 1;
}


use Understand;
use Getopt::Long;
use strict;

report();
sub report {
    return if parse_args();
    return if open_db();
    return if check_language();
    unused_funcs();
}


my ($argCalls,$argDb,$argDef,$argHelp,$argExclude);
sub parse_args {
    GetOptions
	("calls"     => \$argCalls,
	 "db=s"      => \$argDb,
	 "def"       => \$argDef,
	 "exclude=s" => \$argExclude,
	 "help"      => \$argHelp,
	 ) or return usage("");
    return usage("") if ($argHelp);
    $argExclude = build_pattern($argExclude) if $argExclude;
    return $abort;
}


my $db;
sub open_db {
    $db = Understand::Gui::db();
    if (!$db) {
	return usage("Error, database not specified") unless $argDb;
	my $status;
	($db,$status) = Understand::open($argDb);
	return error("Error opening database: $status") if $status;
    }
    return $abort;
}


# check that the db language is supported
my $language;
sub check_language {
    $language = $db->language();
    return error("$language is currently unsupported by this script")
	if ($language !~ /c/i);
    return $abort;
}


# convert a user-specified pattern into an appropriate regex string
sub build_pattern {
    my $pattern = shift;
    return "" unless $pattern;

    # check for word boundaries
    $pattern = "\\b".$pattern if ($pattern !~ /^\*/);
    $pattern = $pattern."\\b" if ($pattern !~ /\*$/);

    # replace wildcards * and ? with regex wildcards .* and ., respectively
    $pattern =~ s/\*/\.\*/g; 
    $pattern=~ s/\?/\./g; 

    return $pattern;
}


# print out all unused functions
sub unused_funcs {
    foreach my $func (sort {$a->longname() cmp $b->longname();}
		      $db->ents("c function ~unresolved ~unknown ~implicit")) {
	next if ($argExclude && $func->longname() =~ /$argExclude/);
	next if ($func->ref("c callby,c useby"));
	next if ($argCalls && $func->ref("c call,c use ptr"));
	next if ($func->longname() eq "main");
	next if (test_operator($func));
	next if (test_virtual_call($func));
	next if (test_overload_call($func));
	print "  ". $func->longname();
	print_def($func) if $argDef;
	print "\n";
    }
}


# return true if entity is an operator
sub test_operator {
    my $ent = shift;
    return 1 if $ent->name() =~ /operator([ ]|[^a-zA-Z0-9_\$])/;
    return 0;
}


# return true if entity is potentially called via a virtual call
sub test_virtual_call {
    my $ent = shift;
    foreach my $ref ($ent->refs("c overrides")) {
	return 1 if $ref->ent->ref("c callby virtual");
    }
    return 0;
}


# return true if entity is potentially called due to overloading
sub test_overload_call {
    my $ent = shift;
    #tbd
    return 0;
}


# print the definition location
sub print_def {
    my $ent = shift;
    my $decl;

    $decl = $ent->ref("definein");
    $decl = $ent->ref("declarein") unless $decl;
    print "  [File: ". $decl->file->longname(). " Line: ". $decl->line(). "]"
	if ($decl);
}
