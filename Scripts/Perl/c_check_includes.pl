#
# Synopsis: Report unneeded #include (C/C++)
#
# Categories: Project Report, Coding Standards
#
# Languages: C
#
# Description:
#  Reports #includes that are unneeded in an Understand for C++ project source.
#  Reports include files that are not directly needed by the file doing the include.
#  Requires an existing Understand for C++ database. 
#

sub usage {
	print shift,"\n";
	print "Usage: check_includes.pl -db database\n";
	print "  -db     database  Specify Understand database if run from uperl.\n";
	print "  -file    Optional, list of specific files to check.\n";
	print "  -inc     Optional, check include files also.\n";
	print "  -v       Optional, show additional info.\n";
	return 1;
}

# Description:
#
#
#  For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl/
#  Refer to the documenation for details on using the perl API 
#  with Understand and for details on all Understand perl API calls.
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

my %cache;
sub check_includes {
	my $files = get_filelist ();
	foreach my $file (@$files) {
		%cache = {};
		my $first = 1;
		foreach my $inc ($file->ents("include","~unresolved ~unknown")) {
			my $used = check_inc_used($file,$inc);
			if ($first && ($argVerbose || !$used)) {
				print $file->longname(). ":\n";
				$first = 0;
			}
			
			#print " check_inc file: ".$file->name()." include: ".$inc->name()." used: ".$used."\n";
			
			if ($argVerbose || !$used) {
				print "  ";
				if ($argVerbose) {
					if (!$used) {
						print "unneeded include ";
					} else {
						print "needed include ";
					}
				}
				print $inc->longname();
				print " (".$used.")" if $argVerbose;
				print "\n";
			}
		}
	}
}


sub check_inc_used {
	my $file = shift;	# toplevel file being checked
	my $inc = shift;	# include file being checked

	# check cache first
	return $cache{$inc->id()} if exists($cache{$inc->id()});

	my $used;
	# check if declare/defines from $inc file are used by $file
	foreach my $fileref ($inc->filerefs("declare, define","c",1)) {
		my $ent = $fileref->ent();

		# check if $ent is directly used
		$used = check_ent_used($file,$ent);
		if ($used) {
			$cache{$inc->id()} = 1;
			return $used;
		}

		# check if implicit members of class $ent are used
		if ($ent->kind->check("c class,c struct,c union")) {
			foreach my $member ($ent->ents("implicit declare")) {
				$used = check_ent_used($file,$member);
				if ($used) {
					$cache{$inc->id()} = $used;
					return $used;
				}
			}
		}
	}

	# mark the cache as unused initially, to break recursion
	$cache{$inc->id()} = 0;

	# check if any local includes are used by the $file
	foreach my $local_inc ($inc->ents("include")) {
		if ($local_inc->kind->check("unresolved,unknown")) {
			return ($cache{$inc->id()} = -1);  # assume it is needed
		} else
		{
			my $loc_inc_used = check_inc_used($file,$local_inc);
			if ($loc_inc_used) {
				return ($cache{$inc->id()} = $loc_inc_used);
			}
		}
	}

	return 0;
}


sub check_ent_used {
	my $file = shift;	# toplevel file being checked
	my $ent = shift;	# entity being checked

	my $refs_count = 0;
	foreach my $ref ($ent->refs("useby,typedby,callby,setby,declarein,definein")) {
		if ($ref->file() == $file) {
			$refs_count +=1;
			#print "\tfile: ".$ref->file()->name()." line:".$ref->line()." name:".$ref->scope()->name()."\n";
		}
	}
	return $refs_count;
}
