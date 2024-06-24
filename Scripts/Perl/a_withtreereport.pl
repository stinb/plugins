# a_withtreereport.pl
#
# Synopsis: Reports the "with-tree " for each top-level entity.
#
# Categories: Project Report
#
# Languages: Ada
#
# usage: a_withtreereport.pl -db <database>
#	database	-	an Understand database
#
# Author: Michael Staheli
# Authored On: 08.02.2005
#
use Understand;
use Getopt::Long;
use strict;

################################################################################

sub Usage {
return << "END_USAGE"
${\(shift @_)}
Usage: a_withtreereport.pl -db <database>
	database	-	an Understand database
END_USAGE
} # end sub Usage ()

################################################################################

my $dbPath;
my $help;

# grab command-line parameters
GetOptions("db=s" => \$dbPath,"help" => \$help);
die Usage("") if($help);

# open the passed Understand database
my $db;
my $status;

if(!$dbPath) {
	$db = Understand::Gui::db();
}else {
	($db, $status) = Understand::open($dbPath);
} # end if(!$dbPath)
die "Error status: $status\n" if($status);

my @adaEntities = $db->ents("entry, function, package, procedure, protected, task");
foreach my $adaEntity (@adaEntities) {
	# filter out all entities that have "withby" relations and are not part of
	# the ada standard libraries
	next if(($adaEntity->ref("withby")) ||
			($adaEntity->library() =~ /standard/i));
	GenerateWithTree($adaEntity);
} # end foreach $adaPackage (@adaPackages)

# close the open Understand database
$db->close() if($dbPath);

################################################################################

sub GenerateWithTree {
	my $adaEntity = shift;
	my @withRefs = $adaEntity->refs("with");
	if(@withRefs) {
		print("+ ".$adaEntity->longname()."\n");

		my @callStack;
		push(@callStack,$adaEntity->longname());
		WalkWiths(\@withRefs,1,\@callStack);
		pop(@callStack);
	} # end if(@withRefs)
} # end sub GenerateWithTree ()

################################################################################

sub WalkWiths {
	my ($withRefs,$depth,$callStack) = @_;
	foreach my $withRef (@$withRefs) {
		my @childWiths = $withRef->ent()->refs("with");

		print ("  " x $depth);
		(@childWiths) ? print("+ ") : print("- ");
		print($withRef->ent()->longname());
		
		if(AlreadyInEntity(\@$callStack,$withRef->ent()->longname())) {
			print(" *** RECURSIVE ***\n");
			next;
		} # end if(AlreadyInEntity($withRef->ent()->longname()))
		print("\n");
		
		push(@$callStack,$withRef->ent()->longname());
		WalkWiths(\@childWiths,($depth+1),\@$callStack);
		pop(@$callStack);
	} # end foreach my $withRef (@$withRefs)
} # end sub WalkWiths ()

################################################################################

sub AlreadyInEntity {
	my ($callStack,$entName) = @_;
	
	# unfortunately we need the call-stack in the order that it's in so we can't
	# sort it to do a quick binary search for $entName
	my $inEntity = 0;
	foreach my $item (@$callStack) {
		if($item eq $entName) {
			$inEntity = 1;
			last;
		} # end if($item eq $entName)
	} # end foreach my $item (@g_callStack)
	return $inEntity;
} # end sub AlreadyInEntity ()

################################################################################
