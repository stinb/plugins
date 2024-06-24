
# Sample Understand PERL API Script
#
# Synopsis: Lists all functions
#
#   Tests for recursive includes
#
# Categories: Project Report, Coding Standards
#
# Languages: C
#
# Usage:
sub usage($) {
	return shift(@_) . <<"END_USAGE";
Usage: $0 -db database
	-db database    Specify Understand database if run from uperl.
	-file           Optional, list of specific files to check.
	-v              Optional, show additional info.
END_USAGE
}
#
#  For the latest Understand perl API documentation, see
#      http://www.scitools.com/perl.html
#
#  15-Nov-2006 KG
#  14-Feb-2014 AL

use Understand;
use Getopt::Long;
use strict;

my $dbPath;
my $help;
my $argVerbose;
my @argFile;
my %tree;
my %recursive;
my @recursivePath;
my %tested;

GetOptions(
	"db=s"   => \$dbPath,
	"help"   => \$help,
	"v"      => \$argVerbose,
	"file=s" => \@argFile,
	);
	@argFile = split(/,/,join(',',@argFile)) if (@argFile);

# help message
die usage("") if ($help);

# open the database
my $db=openDatabase($dbPath);

# check language
if ( $db->language() !~ "C" ) {
	die "This script is designed for C and C++ only\n";
}


#code body*******************************************************************

my $fileList = getFileList();

print @$fileList." file(s) to test\n";

foreach my $file (sort {lc($a->relname()) cmp lc($b->relname());} @$fileList){
	recursiveIncludesCheck($file,0);
}

print "\nfound ".(keys %recursive)." recursive includes:\n" if %recursive;

foreach my $file_id_rec (keys %recursive) {
	print $db->ent_from_id($file_id_rec)->relname." \n";
	printPath(\@{$recursive{$file_id_rec}} );
}

print "\n ".(keys %recursive)." recursive includes found.\n" if %recursive;
print "complete.\n";

#end body********************************************************************
closeDatabase($db);


# subroutines

#Recursive include check
sub recursiveIncludesCheck()
{
	my $file  = shift();
	my $level = shift();
	my $verbose = $argVerbose;

	print $level." ".$file->relname."  tested, skipping ... \n" if $tested{$file->id} && $verbose;
	return if ($tested{$file->id});

	push(@recursivePath, $file->relname);

	print $level." ".$file->relname."  testing ... \n" if $level == 0  && $verbose;
	if ($tree{$file->id}>=1){
		#recursive
		@{$recursive{$file->id}}= @recursivePath;
		print $file->relname."   (recursive)\n" if $verbose;
		printPath (\@{$recursive{$file->id}}) if $verbose;
		return;
	}

	$tree{$file->id} +=1;
	my @includes = $file->refs("include","File ~unknown ~unresolved");
	my @includes_sort = sort {lc($a->ent->relname()) cmp lc($b->ent->relname())} @includes;

	if ($verbose) {
		printDebug($level, $file, \@includes_sort,  \%tested);
	}

	foreach my $inc (@includes_sort) {
		recursiveIncludesCheck($inc->ent,$level+1);
	}

	pop(@recursivePath);
	$tree{$file->id} -= 1;
	$tested{$file->id} = 1;

	if ($level == 0 ) {
		print $level." ".$file->relname."  testing finished ... \n----\n" if $level == 0  && $verbose;
		#cleanup tested after finishing with one file
		#%tested = ();
		@recursivePath = ();
	}

}

sub printPath {
	my $recursivePath = shift;

	my $prefix = "->";
	foreach my $recursiveInc (@$recursivePath) {
		print "\t".$prefix."  ".$recursiveInc."\n";
		$prefix = "-".$prefix;
	}
}


sub printDebug {
	my $level = shift;
	my $file = shift;
	my $includes_sort = shift;
	my $tested = shift;

	my $includes_not_tested = 0;
	foreach my $inc (@$includes_sort) {
		$includes_not_tested ++ if (!$tested->{$inc->ent->id});
	}

	print $level." ".$file->relname;
	if (@$includes_sort) {
		print "  have ".@$includes_sort. " includes ";
	}
	if ($includes_not_tested) {
		print "(".$includes_not_tested." not tested)";
	}
	print "\n";

	foreach my $inc (@$includes_sort) {
		print $level."\t".$inc->ent->relname()." not tested \n" if (!$tested->{$inc->ent->id}) ;
	}
}

sub getFileList {
	my @files;

	if (@argFile) {
		foreach my $name (@argFile) {
			my $file = $db->lookup($name);
			push @files,$file if ($file);
		}
	} else {
		@files = $db->ents("File ~unknown ~unresolved");
	}
	return \@files;
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

