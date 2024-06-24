# a_aggregatemetricsforwithtree.pl - sample Understand perl API program
#
# Synopsis: Reports metrics for each program unit used by a specified "primary"
#			program unit.
#
# Categories: Entity Metrics
#
# Languages: Ada
#
# Usage: a_aggregatemetricsforwithtree.pl -db <database> -startpoint <primary procedure>
#
#   -db      		Understand database
#   -startpoint 	The full name of the primary procedure (application entry point)
#
# Author: Michael Staheli
# Authored On: 09.26.2005
#
use Understand;
use Getopt::Long;
use strict;

################################################################################

sub usage($) {
	return << "END_USAGE";
${ \( shift @_) }
Usage: a_aggregatemetricsforwithtree.pl -db UnderstandDB -startpoint primaryProc

	-db            Understand database
    -startpoint    The full name of the primary procedure (application entry point)
END_USAGE
}

################################################################################

my $dbPath;
my $primaryProc;
my $help;

my %projectMetrics = (
		"CountLine" => 0,
		"CountLineBlank" => 0,
		"CountLineCode" => 0,
		"CountLineComment" => 0,
		"CountLineCodeDecl" => 0,
		"CountLineCodeExe" => 0
);

# grab command-line parameters
GetOptions("db=s" => \$dbPath,"startpoint=s" => \$primaryProc,"help" => \$help);
die Usage("") if($help);

# open the passed Understand database
my $db;
my $status;
if(!$dbPath) {
	$db = Understand::Gui::db();
}else {
	($db,$status) = Understand::open($dbPath);
} # end if(!$dbPath)
die "Error status: $status\n" if($status);
die (usage("You must provide the full name of the primary procedure...\n" ))
    if(!$primaryProc);
    
my $mainProcedure = $db->lookup($primaryProc);
die ("The supplied primary procedure \"$primaryProc\" could not be found!\n")
    if(!$mainProcedure);

print("Primary Procedure: ".$mainProcedure->longname()."\n\n");
my %entUniqueWiths = GetUniqueWiths($mainProcedure);

print "\n\n". $mainProcedure->longname(). " is built from these program units:\n\n";

my $numItems = 0;
foreach my $key (keys(%entUniqueWiths)) {
	my $entity = $entUniqueWiths{$key};
	ShowEntMetrics($entity,\%projectMetrics);

	print("\n");
	$numItems++;
} # end foreach my $key (keys(%entUniqueWiths))

# display aggregate metrics
print("*" x 80);
print("\n");
ShowProjectMetrics(\%projectMetrics) if($numItems);

# close the open Understand database
$db->close() if($dbPath);

################################################################################

sub GetUniqueWiths {
	my $adaEntity = shift;
	my @entRefs;
	my %entUniqueWiths;
	
	my @withRefs = $adaEntity->refs("with");
	if(@withRefs && AddWithEntity(\%entUniqueWiths,$adaEntity)) {
		my @callStack;
		push(@callStack,$adaEntity->longname());
		
		WalkWiths(\@withRefs,1,\@callStack,\%entUniqueWiths);
		pop(@callStack);
	} # end if(@withRefs && AddWithEntity(\%entUniqueWiths,$adaEntity))
	return %entUniqueWiths;
} # end sub GetUniqueWiths ()

################################################################################

sub WalkWiths {
	my ($withRefs,$depth,$callStack,$entUniqueWiths) = @_;
	foreach my $withRef (@$withRefs) {
		next if($withRef->ent()->library() =~ /standard/i);
		my @childWiths = $withRef->ent()->refs("with");

		AddWithEntity(\%$entUniqueWiths,$withRef->ent())
			if(!AlreadyInEntity(\@$callStack,$withRef->ent()->longname()));
		
		push(@$callStack,$withRef->ent()->longname());
		WalkWiths(\@childWiths,($depth+1),\@$callStack,\%$entUniqueWiths);
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

sub AddWithEntity {
	my ($withEnts, $entity) = @_;

	my $entityAdded = 0;
	if(!%$withEnts->{$entity->id()}) {
		$entityAdded = 1;
		%$withEnts->{$entity->id()} = $entity;
	} # end if(!%$withEnts->{$entity->id()})
	return $entityAdded;
} # end sub AddWithEntity ()

################################################################################

sub ShowEntMetrics {
	my ($entity,$projectMetrics) = @_;
	print($entity->longname().":\n");

	my $linesCode = 0;
	my $linesComment = 0;

	my $crntMetric = "CountLine";	
	my $crntPackageValue = $entity->metric($crntMetric);
	my $crntProjectValue = %$projectMetrics->{$crntMetric};
	%$projectMetrics->{$crntMetric} = ($crntProjectValue + $crntPackageValue);
	print("\tLines: $crntPackageValue\n");
									  
	$crntMetric = "CountLineBlank";	
	$crntPackageValue = $entity->metric($crntMetric);
	$crntProjectValue = %$projectMetrics->{$crntMetric};
	%$projectMetrics->{$crntMetric} = ($crntProjectValue + $crntPackageValue);
	print("\tLines Blank: $crntPackageValue\n");

	$crntMetric = "CountLineCode";	
	$crntPackageValue = $entity->metric($crntMetric);
	$crntProjectValue = %$projectMetrics->{$crntMetric};
	%$projectMetrics->{$crntMetric} = ($crntProjectValue + $crntPackageValue);
	print("\tLines Code: $crntPackageValue\n");
	$linesCode = $crntPackageValue;
	
	$crntMetric = "CountLineComment";	
	$crntPackageValue = $entity->metric($crntMetric);
	$crntProjectValue = %$projectMetrics->{$crntMetric};
	%$projectMetrics->{$crntMetric} = ($crntProjectValue + $crntPackageValue);
	print("\tLines Comment: $crntPackageValue\n");
	$linesComment = $crntPackageValue;
	
	$crntMetric = "CountLineCodeDecl";	
	$crntPackageValue = $entity->metric($crntMetric);
	$crntProjectValue = %$projectMetrics->{$crntMetric};
	%$projectMetrics->{$crntMetric} = ($crntProjectValue + $crntPackageValue);
	print("\tDeclarative Statements: $crntPackageValue\n");

	$crntMetric = "CountLineCodeExe";	
	$crntPackageValue = $entity->metric($crntMetric);
	$crntProjectValue = %$projectMetrics->{$crntMetric};
	%$projectMetrics->{$crntMetric} = ($crntProjectValue + $crntPackageValue);
	print("\tExecutable Statements: $crntPackageValue\n");
	
	my $commentCodeRatio = 0;
	$commentCodeRatio = ($linesComment/$linesCode) if($linesCode);
	print("\tRatio Comment/Code: ".$commentCodeRatio."\n");
} # end sub ShowEntMetrics ()

################################################################################

sub ShowProjectMetrics {
	my $projectMetrics = shift;
	my $linesCode = %$projectMetrics->{"CountLineCode"};
	my $linesComment = %$projectMetrics->{"CountLineComment"};
	
	print("Summary of metrics for units building: " . $mainProcedure->longname() . "\n\n");
	print("Lines: ".%$projectMetrics->{"CountLine"}."\n");
	print("Lines Blank: ".%$projectMetrics->{"CountLineBlank"}."\n");
	print("Lines Code: ".$linesCode."\n");
	print("Lines Comment: ".$linesComment."\n");
	print("Declarative Statements: ".%$projectMetrics->{"CountLineCodeDecl"}."\n");
	print("Executable Statements: ".%$projectMetrics->{"CountLineCodeExe"}."\n");
	
	my $commentCodeRatio = 0;
	($commentCodeRatio = ($linesComment/$linesCode)) if($linesCode);
	print("Ratio Comment/Code: ".$commentCodeRatio."\n");
} # end sub ShowProjectMetrics ()
