# Synopsis: Report an alternate FANIN/FANOUT metric for C++ functions.
#
# Categories: Project Metrics
#
# Languages: C
#
# Description:
#  Report an alternate FANIN/FANOUT metric for c/c++ functions.
#
#  FANIN is the count of unique functions that call a given function,
#    either directly, or ultimately, via other functions.
#  FANOUT is the count of unique functions that are called by a
#    given function, either directly, or ultimately, via other
#    functions.
#
#  The metrics may be calculated 'per file', which means that only
#    values are reported for files. The file values are based on
#    the all unique calls (or called by) of all the functions defined
#    in the file, and excludes calls (or called by) to functions within
#    the file.
#
#   For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl/
#

sub usage($) {
    print shift,"\n";
    print "Usage: c_fanin_fanout.pl\n";
    print "  -db database   Specify the Understand database if run from uperl\n";
    print "  -csv           Output in a format suitable for spredsheet import\n";
    print "  -fanin         Report FANIN metric\n";
    print "  -fanout        Report FANOUT metric\n";
    print "  -perfile       Specify calculations and output to be file-based\n";
}

use Understand;
use Getopt::Long;
use strict;

my ($argCsv,$argFanin,$argFanout,$argVerbose,$argVerbose2,$db);
report();


sub report() {
    # get options
    my ($argDb,$argHelp,$argPerfile);
    GetOptions
	("db=s"        => \$argDb,
	 "help"        => \$argHelp,
	 "csv"         => \$argCsv,
	 "fanin"       => \$argFanin,
	 "fanout"      => \$argFanout,
	 "perfile"     => \$argPerfile,
	 "v|verbose"   => \$argVerbose,
	 "v2|verbose2" => \$argVerbose2,
	 ) or return usage("");
    return usage("") if ($argHelp);
    $argFanin = $argFanout = 1 unless ($argFanin || $argFanout);
    $argVerbose = 1 if $argVerbose2;

    # open the database
    $db = Understand::Gui::db();
    if (!$db) {
	return usage("Error, database not specified") unless $argDb;
	my $status;
	($db,$status) = Understand::open($argDb);
	return print("Error opening database: $status\n") if $status;
    }

    # check language
    my $language = $db->language();
    return print("$language is currently unsupported") if ($language !=/c/i);

    if ($argPerfile) {
	per_file();
    } else {
	per_function();
    }
}


sub per_function {
    # calculate metrics for each function
    my (%fanin,%fanout);
    foreach my $func ($db->ents("function ~unknown ~unresolved")) {
	if ($argFanin) {
	    my %used;
	    print "Calculating FANIN for ". $func->longname(). "\n" if $argVerbose;
	    $used{$func->id()} = 1;
	    $fanin{$func->id()} = calc_fanin($func,\%used);
	}
	if ($argFanout) {
	    my %used;
	    print "Calculating FANOUT for ". $func->longname(). "\n" if $argVerbose;
	    $used{$func->id()} = 1;
	    $fanout{$func->id()} = calc_fanout($func,\%used);
	}
    }
    print "\n" if $argVerbose;

    # output metrics
    if ($argCsv) {
	print "Name";
	print ",FANIN" if ($argFanin);
	print ",FANOUT" if ($argFanout);
	print "\n";
    }
    foreach my $func (sort {
	lc($a->longname()) cmp lc($b->longname())}
		      $db->ents("function ~unknown ~unresolved")) {
	if ($argCsv) {
	    print $func->longname();
	    print ",". $fanin{$func->id()} if ($argFanin);
	    print ",". $fanout{$func->id()} if ($argFanout);
	    print "\n";
	} elsif ($argFanin && !$argFanout) {
	    print " ". $func->longname(). " = ". $fanin{$func->id()}. "\n";
	} elsif ($argFanout && !$argFanin) {
	    print " ". $func->longname(). " = ". $fanout{$func->id()}. "\n";
	} else {
	    print " ". $func->longname(). "\n";
	    print "   FANIN  = ". $fanin{$func->id()}. "\n";
	    print "   FANOUT = ". $fanout{$func->id()}. "\n";
	}
    }
}


sub per_file {
    # calculate metrics for each file
    my (%fanin,%fanout);
    foreach my $file ($db->ents("code file")) {
	# get list of functions defined in file
	my @funcs;
	foreach my $ref ($file->filerefs("define","function")) {
	    push @funcs,$ref->ent();
	}
	# calculate FANIN
	if ($argFanin) {
	    print "Calculating FANIN for ". $file->longname(). "\n" if $argVerbose;
	    my %used;
	    foreach my $func (@funcs) {
		$used{$func->id()} = 1;
	    }
	    foreach my $func (@funcs) {
		print "  Function: ". $func->longname(). "\n" if $argVerbose2;
		$fanin{$file->id()} += calc_fanin($func,\%used);
	    }
	}
	# calculate FANOUT
	if ($argFanout) {
	    print "Calculating FANOUT for ". $file->longname(). "\n" if $argVerbose;
	    my %used;
	    foreach my $func (@funcs) {
		$used{$func->id()} = 1;
	    }
	    foreach my $func (@funcs) {
		print "  Function: ". $func->longname(). "\n" if $argVerbose2;
		$fanout{$file->id()} += calc_fanout($func,\%used);
	    }
	}
    }
    print "\n" if $argVerbose;

    # output metrics
    if ($argCsv) {
	print "Name";
	print ",FANIN" if ($argFanin);
	print ",FANOUT" if ($argFanout);
	print "\n";
    }
    foreach my $file (sort {
	lc($a->longname()) cmp lc($b->longname())}
		      $db->ents("code file")) {
	if ($argCsv) {
	    print $file->longname();
	    print ",". $fanin{$file->id()} if ($argFanin);
	    print ",". $fanout{$file->id()} if ($argFanout);
	    print "\n";
	} elsif ($argFanin && !$argFanout) {
	    print " ". $file->longname(). " = ". $fanin{$file->id()}. "\n";
	} elsif ($argFanout && !$argFanin) {
	    print " ". $file->longname(). " = ". $fanout{$file->id()}. "\n";
	} else {
	    print " ". $file->longname(). "\n";
	    print "   FANIN  = ". $fanin{$file->id()}. "\n";
	    print "   FANOUT = ". $fanout{$file->id()}. "\n";
	}
    }
}


sub calc_fanin {
    my $func = shift;
    my $used = shift;
    my $count = 0;

    foreach my $ref ($func->refs("callby ~inactive,useby ~inactive","",1)) {
	if (!exists $$used{$ref->ent->id()}) {
	    print "   Calledby ". $ref->ent->longname(). "\n" if $argVerbose2;
	    $$used{$ref->ent->id()} = 1;
	    $count = $count + 1 + calc_fanin($ref->ent(),$used);
	}
    }
    return $count;
}


sub calc_fanout {
    my $func = shift;
    my $used = shift;
    my $count = 0;

    foreach my $ref ($func->refs("call ~inactive","",1)) {
	if (!exists $$used{$ref->ent->id()}) {
	    print "   Calls ". $ref->ent->longname(). "\n" if $argVerbose2;
	    $$used{$ref->ent->id()} = 1;
	    $count = $count + 1 + calc_fanout($ref->ent(),$used);
	}
    }
    return $count;
}
