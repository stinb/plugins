#
# Synopsis: Report needed #include (C/C++)
#
# Categories: Project Report, Coding Standards
#
# Languages: C
#
# Description:
#  Reports #includes that are needed for C++ file and for which entry.
#  Requires an existing Understand for C++ database. 
#
# Usage:
sub usage {
	print shift,"\n";
	print "Usage: check_includes.pl -db database\n";
	print "  -db     database  Specify Understand database if run from uperl.\n";
	print "  -file    Optional, list of specific files to check.\n";
	print "  -inc     Optional, check include files also.\n";
	print "  -v       Optional, show additional info.\n";
	return 1;
}
#  For the latest Understand perl API documentation, see
#      http://www.scitools.com/perl.html
#
#  14-Feb-2014 AL

my $abort=0;
sub error {
	print shift,"\n";
	$abort = 1;
	return 1;
}


use Understand;
use Getopt::Long;
use FileHandle; autoflush STDOUT 1;
use strict;

report();
sub report {
	return if parse_args();
	return if open_db();
	return if check_language();
	check_includes();
}


my ($argDb,@argFile,$argHelp,$argInc,$argVerbose);
sub parse_args {
	GetOptions
	("db=s"   => \$argDb,
	 "file=s" => \@argFile,
	 "help"   => \$argHelp,
	 "inc"    => \$argInc,
	 "v"      => \$argVerbose,
	 ) or return usage("");
	return usage("") if ($argHelp);
	@argFile = split(/,/,join(',',@argFile)) if (@argFile);
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


my $language;
sub check_language {
	$language = $db->language();
	return error("$language is currently unsupported by this script")
	if ($language !~ /c/i);
		return $abort;
}


sub get_filelist {
	my @files;
	if (@argFile) {
		foreach my $name (@argFile) {
			my $file = $db->lookup($name);
			push @files,$file if ($file);
		}
	} elsif ($argInc) {
		@files = $db->ents("c file ~unknown ~unresolved");
	} else {
		@files = $db->ents("c code file"); 
	}
	return \@files;
}

sub check_includes {
	my $files = get_filelist ();
	foreach my $file (sort {$a->longname() cmp $b->longname()} @$files) {
		print "checking file: ".$file->longname()."\n";
		check_inc_needed($file);
	}
}


sub check_inc_needed {
	my $file = shift;	# toplevel file being checked

	my $file_depends = $file->depends();

	if  ($file_depends) {
		print " file: ".$file->name()." depends: ".$file_depends->values()."\n" if $argVerbose;

		foreach my $ent ($file_depends->keys()) {
			print "\t#include \"".$ent->name()."\"";
			if ($argVerbose) {
				print "  (".$file_depends->value($ent).")  :\t";
				my %entries;
				foreach my $val ($file_depends->value($ent)) {
					if ($val->kindname() ne "Include") {
						$entries{$val->ent()->name()} += 1;
					}
				}

				foreach my $entry (keys %entries) {
					print " ".$entry." (".$entries{$entry}.")";
				}
			}
			print "\n";
		}
	}
}
