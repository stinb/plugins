# cj_classcomplexityvariance.pl
#
# Synopsis: output the variance and standard deviation of the complexity for all of the classes methods(functions).
# For each not unknown, not unnamed and not unresolved class declared in the Understand database, 
#
#	For each class, the
#		- variance
#		- standard deviation
#		- average complexity
#		- number of methods/functions
#		- sigma (used to calculate variance/standard dev)
#	is output.
#
# usage: classcomplexityvariance.pl -db <database> [-cvs]
#	database	-	an Understand database
#	cvs			-	format the output as comma delimited text for import into
#					a MS Access database or MS Excel spreadsheet
#
# Categories: Project Metrics
#
# Languages: C, Java
#
# Author: Michael Staheli
# Authored On: 08.18.2005
#
use Understand;
use Getopt::Long;
use strict;

################################################################################

sub Usage {
return << "END_USAGE"
${\(shift @_)}
 usage: classcomplexityvariance.pl -db <database>
	database	-	an Understand database
	cvs			-	format output for cvs
END_USAGE
} # end sub Usage ()

################################################################################

my $dbPath;
my $help;
my $g_cvs;

# grab command-line parameters
GetOptions("db=s" => \$dbPath,"help" => \$help, "cvs" => \$g_cvs);
die Usage("") if($help);

# open the passed Understand database
my $db;
my $status;
if(!$dbPath) {
	$db = Understand::Gui::db();
	die "Error obtaining database from Understand!" if(!$db);
}else {
	($db, $status) = Understand::open($dbPath);
} # end if($dbPath == undef)
die "Error status: $status\n" if($status);

# display a header if output is intended for cvs
if($g_cvs) {
	print("Class,Variance,Std. Deviation,Avg. Complexity,Num. Methods,Sigma\n");
} # end if($g_cvs)

# grab all the classes in the Understand project
my @classes = $db->ents("class ~unknown ~unresolved, interface ~unknown ~unresolved");
my $totalClasses = 0;

foreach my $class (@classes) {
	next if ($class->library() =~ /standard/i);
	
	$totalClasses++;
	CalculateVariance($class);
} # end foreach my $class (@classes)
print("Total classes: $totalClasses\n");

# close the open Understand database
$db->close() if($dbPath);

################################################################################

sub CalculateVariance {	
	# get a reference for each method declared/defined in $class
	my $class = shift;
	my @classMethods = $class->refs("declare,define","function,method ~unknown ~unresolved");
	
	# start the computational process for the variance of the class
	my $avgMcCabeVal = 0;	
	my %methodComplexities = {};
	my $numMethods = scalar(@classMethods);

	# compute the average complexity and store the complexity for each method
	# declared in $class
	foreach my $classMethod (@classMethods) {
		my $methodEnt = $classMethod->ent();
		my $crntMcCabeVal = $methodEnt->metric("Cyclomatic");

		$avgMcCabeVal += $crntMcCabeVal;
		$methodComplexities{$methodEnt->id()} = $crntMcCabeVal;
	} # end foreach my $classMethod (@classMethods)
	
	# compute class stats
	if($numMethods) {
		$avgMcCabeVal /= $numMethods;	# now this is the average and not a sumation
		ComputeDisplayStats($class,\@classMethods,\%methodComplexities,
							$avgMcCabeVal,$numMethods);
	}else {
		ComputeDisplayStats($class,\@classMethods,\%methodComplexities,
							0,0);
	} # end if($numMethods)
} # end sub CalculateVariance ()

################################################################################

sub ComputeDisplayStats {
	my ($class,$classMethods,$methodComplexities,$avgMcCabeVal,$numMethods) = @_;
	my %complexities = %$methodComplexities;
	
	my $sigma = 0;
	foreach my $classMethod (@$classMethods) {
		my $methodEnt = $classMethod->ent();
		my $complexity =$complexities{$methodEnt->id()};
		$sigma += (($complexity - $avgMcCabeVal) *
					($complexity - $avgMcCabeVal));
	} # end foreach my $classMethod (@$classMethods)
	
	# compute variance & standard deviation
	my $variance = 0;
	if(($numMethods - 1) > 0) {
		$variance = ($sigma / ($numMethods - 1));
	} # end if(($numMethods - 1) > 0)
	my $stdDeviation = sqrt($variance);

	if($g_cvs) {
		print($class->name().
			  ",$variance,$stdDeviation,$avgMcCabeVal,$numMethods,$sigma");
	}else {
		print("Class: ".$class->name()."\n");
		print("\tVariance: $variance\n");
		print("\tStd. Deviation: $stdDeviation\n");
		print("\tAvg. Complexity: $avgMcCabeVal\n");
		print("\tNum. Methods: $numMethods\n");
		print("\tSigma: $sigma\n");
	} # end if($g_cvs)
	print("\n");
} # end sub ComputeStats ()

################################################################################
