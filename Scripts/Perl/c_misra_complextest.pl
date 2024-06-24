# c_misra_complextest.pl
#
# Synopsis: Reports MISRA complexity & testability metrics
#
# Categories: Project Metrics
#
# Languages: C
#
# MISRA complexity & testability metrics.
#
# - Modified McCabe complexity
# - Component stress complexity
# - Essential cyclomatic number
# - Number of execution paths
# - Number of exit points
# - Structural complexity
# - hierarchical complexity
#
use Understand;
use Getopt::Long;
use strict;

# defined threshold values for metric reporting
use constant MAX_MODIFIED_MCCABE_COMPLEXITY     => 15;
use constant MAX_COMPONENT_STRESS_COMPLEXITY    => 10000;
use constant MAX_ESSENTIAL_CYCLOMATIC_NUMBER    => 1;
use constant MAX_NUMBER_OF_EXECUTION_PATHS      => 75;
use constant MAX_NUMBER_OF_EXIT_POINTS          => 1;
use constant MAX_STRUCTURAL_COMPLEXITY          => 15;
use constant MAX_HIERARCHICAL_COMPLEXITY        => 5;

#------------------------------------------------------------------------------#

my $dbPath;
GetOptions("db=s" => \$dbPath);

# open the Understand database
my $db = OpenDatabase($dbPath);
die unless($db);

# result arrays
my @modifiedMcCabeComplexity;
my @componentStressComplexity;
my @essentialCyclomaticNumber;
my @numExecutionPaths;
my @numExitPoints;
my @structuralComplexity;
my @hierarchicalComplexity;

# look at each file in the project
foreach my $fileEnt ($db->ents("file ~unknown ~unresolved")) {
#print($fileEnt->name()."\n");
    # skip standard library functions
    next if ($fileEnt->library() =~ /standard/i);
    
    # grab the lexer for the file
    my ($lexer,$status) = $fileEnt->lexer();
    die $status if($status);
    
    # get all functions defined in $fileEnt
    my @functions = GetFunctions($db,$fileEnt,$lexer);    
    foreach my $function (@functions) {
       	my $startRef = GetDeclRef($function,$db->language());
        my $endRef = $function->refs("end");
        unless($startRef and $endRef) {
            print "no endref\n";
            next;
        } # end unless($startRef and $endRef)
        
        # check modified McCabe complexity
        my $metricVal = $function->metric("CyclomaticModified");
        if($metricVal > MAX_MODIFIED_MCCABE_COMPLEXITY) {
            my $badFunc = {};
            $badFunc->{"fileName"} = $fileEnt->name();
            $badFunc->{"funcName"} = $function->name();
            $badFunc->{"value"} = $metricVal;
            $badFunc->{"limit"} = MAX_MODIFIED_MCCABE_COMPLEXITY;
            push(@modifiedMcCabeComplexity,$badFunc);
        } # end if($metricVal > MAX_MODIFIED_MCCABE_COMPLEXITY)
        
        # check component stress complexity
        my %usedIn;
        my %usedOut;
        $usedIn{$function->id()} = 1;
        $usedOut{$function->id()} = 1;
#        $metricVal *= CalcFanIn($function,\%usedIn);
#        $metricVal *= CalcFanOut($function,\%usedOut);
        $metricVal *= $function->metric("CountInput");
        $metricVal *= $function->metric("CountOutput");
        if($metricVal > MAX_COMPONENT_STRESS_COMPLEXITY) {
            my $badFunc = {};
            $badFunc->{"fileName"} = $fileEnt->name();
            $badFunc->{"funcName"} = $function->name();
            $badFunc->{"value"} = $metricVal;
            $badFunc->{"limit"} = MAX_COMPONENT_STRESS_COMPLEXITY;
            push(@componentStressComplexity,$badFunc);
        } # end if($metricVal > MAX_COMPONENT_STRESS_COMPLEXITY)
        
        # check essential cyclomatic number
        $metricVal = $function->metric("Essential");
        if($metricVal > MAX_ESSENTIAL_CYCLOMATIC_NUMBER) {
            my $badFunc = {};
            $badFunc->{"fileName"} = $fileEnt->name();
            $badFunc->{"funcName"} = $function->name();
            $badFunc->{"value"} = $metricVal;
            $badFunc->{"limit"} = MAX_ESSENTIAL_CYCLOMATIC_NUMBER;
            push(@essentialCyclomaticNumber,$badFunc);
        } # end if($metricVal > MAX_ESSENTIAL_CYCLOMATIC_NUMBER)
        
        # check number of execution paths
        $metricVal = $function->metric("CountPath");
        if($metricVal > MAX_NUMBER_OF_EXECUTION_PATHS) {
            my $badFunc = {};
            $badFunc->{"fileName"} = $fileEnt->name();
            $badFunc->{"funcName"} = $function->name();
            $badFunc->{"value"} = $metricVal;
            $badFunc->{"limit"} = MAX_NUMBER_OF_EXECUTION_PATHS;
            push(@numExecutionPaths,$badFunc);
        } # end if($metricVal > MAX_NUMBER_OF_EXECUTION_PATHS)
        
        # check number of exit points
        $metricVal = GetNumExitPoints($function,$lexer,$startRef->line(),$endRef->line());
        if($metricVal > MAX_NUMBER_OF_EXIT_POINTS) {
            my $badFunc = {};
            $badFunc->{"fileName"} = $fileEnt->name();
            $badFunc->{"funcName"} = $function->name();
            $badFunc->{"value"} = $metricVal;
            $badFunc->{"limit"} = MAX_NUMBER_OF_EXIT_POINTS;
            push(@numExitPoints,$badFunc);
        } # end if($metricVal > MAX_NUMBER_OF_EXIT_POINTS)
        
        # check structural complexity
        if(!$function->refs("callby")) {
#print("\t".$function->name()."\n");
            my %usedFuncs;
            my $depth = 0;
            my $numFuncCalls = CountFuncCalls($function,\%usedFuncs,\$depth);
            my $numFuncs = (scalar(keys(%usedFuncs)) - 1);
#print("\t\tCalls: $numFuncCalls -- Funcs: $numFuncs -- Depth: $depth\n");
            if($numFuncs) {
                $metricVal = int($numFuncCalls/$numFuncs);
                if($metricVal > MAX_STRUCTURAL_COMPLEXITY) {
                    my $badFunc = {};
                    $badFunc->{"fileName"} = $fileEnt->name();
                    $badFunc->{"funcName"} = $function->name();
                    $badFunc->{"value"} = $metricVal;
                    $badFunc->{"limit"} = MAX_STRUCTURAL_COMPLEXITY;
                    push(@structuralComplexity,$badFunc);
                } # end if($metricVal > MAX_STRUCTURAL_COMPLEXITY)
            } # end if($numFuncs)
            
            if($depth) {
                $metricVal = int($numFuncs/$depth);
                if($metricVal > MAX_HIERARCHICAL_COMPLEXITY) {
                    my $badFunc = {};
                    $badFunc->{"fileName"} = $fileEnt->name();
                    $badFunc->{"funcName"} = $function->name();
                    $badFunc->{"value"} = $metricVal;
                    $badFunc->{"limit"} = MAX_HIERARCHICAL_COMPLEXITY;
                    push(@hierarchicalComplexity,$badFunc);
                } # end if($metricVal > MAX_HIERARCHICAL_COMPLEXITY)
            } # end if($depth)
        } # end if(!$function->refs("callby"))
    } # end foreach my $function (@functions)    
} # end foreach my $fileEnt ($db->ents("file ~unknown ~unresolved"))

# display results
if(@modifiedMcCabeComplexity) {
    print("Functions exceeding the Modified McCabe Complexity [".
            MAX_MODIFIED_MCCABE_COMPLEXITY."]\n");
    foreach my $badFunc (@modifiedMcCabeComplexity) {
        print("\t".
              $badFunc->{"fileName"}.", ".
              $badFunc->{"funcName"}.", ".
              $badFunc->{"value"}.", ".
              $badFunc->{"limit"}.
              "\n"
            );
    } # end foreach my $badFunc (@modifiedMcCabeComplexity)
    print("\n");
} # end if(@modifiedMcCabeComplexity)
if(@componentStressComplexity) {
    print("Functions exceeding Component Stress Complexity [".
            MAX_COMPONENT_STRESS_COMPLEXITY."]\n");
    foreach my $badFunc (@componentStressComplexity) {
        print("\t".
              $badFunc->{"fileName"}.", ".
              $badFunc->{"funcName"}.", ".
              $badFunc->{"value"}.", ".
              $badFunc->{"limit"}.
              "\n"
            );
    } # end foreach my $badFunc (@componentStressComplexity)
    print("\n");
} # end if(@componentStressComplexity)
if(@essentialCyclomaticNumber) {
    print("Functions exceeding the Essential Cyclomatic Number [".
            MAX_ESSENTIAL_CYCLOMATIC_NUMBER."]\n");
    foreach my $badFunc (@essentialCyclomaticNumber) {
        print("\t".
              $badFunc->{"fileName"}.", ".
              $badFunc->{"funcName"}.", ".
              $badFunc->{"value"}.", ".
              $badFunc->{"limit"}.
              "\n"
            );
    } # end foreach my $badFunc (@essentialCyclomaticNumber)
    print("\n");
} # end if(@essentialCyclomaticNumber)
if(@numExecutionPaths) {
    print("Functions exceeding the Number of Execution Paths [".
            MAX_NUMBER_OF_EXECUTION_PATHS."]\n");
    foreach my $badFunc (@numExecutionPaths) {
        print("\t".
              $badFunc->{"fileName"}.", ".
              $badFunc->{"funcName"}.", ".
              $badFunc->{"value"}.", ".
              $badFunc->{"limit"}.
              "\n"
            );
    } # end foreach my $badFunc (@numExecutionPaths)
    print("\n");
} # end if(@numExecutionPaths)
if(@numExitPoints) {
    print("Functions exceeding the Number of Exit Points [".
            MAX_NUMBER_OF_EXIT_POINTS."]\n");
    foreach my $badFunc (@numExitPoints) {
        print("\t".
              $badFunc->{"fileName"}.", ".
              $badFunc->{"funcName"}.", ".
              $badFunc->{"value"}.", ".
              $badFunc->{"limit"}.
              "\n"
            );
    } # end foreach my $badFunc (@numExitPoints)
    print("\n");
} # end if(@numExitPoints)
if(@structuralComplexity) {
    print("Functions exceeding Structural Complexity [".
            MAX_STRUCTURAL_COMPLEXITY."]\n");
    foreach my $badFunc (@structuralComplexity) {
        print("\t".
              $badFunc->{"fileName"}.", ".
              $badFunc->{"funcName"}.", ".
              $badFunc->{"value"}.", ".
              $badFunc->{"limit"}.
              "\n"
            );
    } # end foreach my $badFunc (@numExitPoints)
    print("\n");
} # end if(@structuralComplexity)
if(@hierarchicalComplexity) {
    print("Functions exceeding Hierarchical Complexity [".
            MAX_HIERARCHICAL_COMPLEXITY."]\n");
    foreach my $badFunc (@hierarchicalComplexity) {
        print("\t".
              $badFunc->{"fileName"}.", ".
              $badFunc->{"funcName"}.", ".
              $badFunc->{"value"}.", ".
              $badFunc->{"limit"}.
              "\n"
            );
    } # end foreach my $badFunc (@numExitPoints)
    print("\n");
} # end if(@hierarchicalComplexity)

#------------------------------------------------------------------------------#

sub OpenDatabase {
    my $dbPath = shift;
    my $db = Understand::Gui::db();

    # path not allowed if opened by understand
    die "database already opened by GUI, don't use -db option\n"
            if($db && $dbPath);
            
    # open database if not already open
    if(!$db) {
        my $status;
        die usage("Error, database not specified\n\n") unless ($dbPath);
	
        ($db,$status)=Understand::open($dbPath);
        die "Error opening database: ",$status,"\n" if $status;
    } # end if(!$db)
    return $db;
} # end sub OpenDatabase ()

#------------------------------------------------------------------------------#

sub CloseDatabase {
    my $db =shift;

    # close database only if we opened it
    $db->close() if ($dbPath);
} # end sub CloseDatabase ()

#------------------------------------------------------------------------------#

sub GetFunctions {
    my ($db,$fileEnt,$lexer) = @_;
    my $language = $db->language();   # use language of $fileEnt when available
    my @functions;

    my $refKind;
    my $entKind;
    if($language =~ /ada/i) {
        $refKind = "declarein body";
        $entKind = "function,procedure";
    }elsif($language =~ /java/i) {
        $refKind = "definein";
    	$entKind = "method";
    }elsif($language =~ /c/i) {
        $refKind = "definein";
        $entKind = "function";
    }else{
        return;
    } # end if($language =~ /ada/i)

    foreach my $lexeme ($lexer->lexemes()) {
        next if(!$lexeme);
        
        my $ref = $lexeme->ref();
        my $ent = $lexeme->entity() if $ref;
        if($ref && $ent &&
                $ref->kind->check($refKind) &&
                $ent->kind->check($entKind)) {
            push(@functions,$ent);
        } # end if(...)
    } # end foreach my $lexeme ($lexer->lexemes())
    return @functions;
} # end sub GetFunctions ()

#------------------------------------------------------------------------------#

sub GetDeclRef {
    my $ent = shift;
    my $language = shift;
    my $decl;
    return $decl unless defined ($ent);
    
    if($language =~ /ada/i) {
        ($decl) = $ent->refs("declarein body");
    }elsif($language =~ /c|fortran|java/i) {
        ($decl) = $ent->refs("definein");
        ($decl) = $ent->refs("declarein") unless ($decl);
    } # end if($language =~ /ada/i)
    return $decl;
} # end sub GetDeclRef ()

#------------------------------------------------------------------------------#

sub GetNumExitPoints {
    my ($funcEnt,$lexer,$startLine,$endLine) = @_;
    my $numExitPoints = 0;
    
    # if this is a proceduce (a function that doesn't have a return type)
    # then we automatically assume one exit point for the end of the
    # procedure. this assumption is made because it is very uncommon (even
    # quite odd) for procedures to have an explicit return statement at
    # the end of their definition.
    $numExitPoints++ if(!$funcEnt->type() || ($funcEnt->type() eq "void"));
	foreach my $lexeme ($lexer->lexemes($startLine,$endLine)) {
		$numExitPoints++ if(($lexeme->token() eq "Keyword") &&
                                ($lexeme->text() eq "return"));
    } # end foreach my $lexeme ($lexer->lexemes($startLine,$endLine))
					
	# this should never happen...but we'll cover it anyway
	$numExitPoints = 1 if($numExitPoints == 0);
    return $numExitPoints;
} # end sub GetNumExitPoints ()

#------------------------------------------------------------------------------#

sub CalcFanIn {
    my ($funcEnt,$used) = @_;
    my $count = 0;

    foreach my $ref ($funcEnt->refs("callby ~inactive,useby ~inactive","",1)) {
        if(!exists $$used{$ref->ent->id()}) {
            $$used{$ref->ent->id()} = 1;
            $count = $count + 1 + CalcFanIn($ref->ent(),$used);
        } # end if(!exists $$used{$ref->ent->id()})
    } # end foreach my $ref ($funcEnt->refs("callby ~inactive,useby ~inactive","",1))
    return $count;    
} # end sub CalcFanIn ()

#------------------------------------------------------------------------------#

sub CalcFanOut {
    my ($funcEnt,$used) = @_;
    my $count = 0;

    foreach my $ref ($funcEnt->refs("call ~inactive","",1)) {
        if(!exists $$used{$ref->ent->id()}) {
            $$used{$ref->ent->id()} = 1;
            $count = $count + 1 + CalcFanOut($ref->ent(),$used);
        } # end if(!exists $$used{$ref->ent->id()})
    } # end foreach my $ref ($funcEnt->refs("call ~inactive","",1))
    return $count;
} # end sub CalcFanOut ()

#------------------------------------------------------------------------------#

sub CountFuncCalls {
    my ($funcEnt,$usedFuncs,$depth) = @_;
    return 0 if(exists($usedFuncs->{$funcEnt->id()}));

    # add $funcEnt to used function list
    $usedFuncs->{$funcEnt->id()} = 1;
    my $crntDepth = $$depth;
    
    my $maxDepth = 0;
    my $numFuncCalls = 0;
    
    foreach my $funcRef ($funcEnt->refs("call")) {
        $numFuncCalls++;

        my $tmpDepth = $crntDepth;
        $numFuncCalls += CountFuncCalls($funcRef->ent(),$usedFuncs,\$tmpDepth);
        $maxDepth = $tmpDepth if($tmpDepth > $maxDepth);
    } # end foreach my $funcRef (@callRefs)
    
    # there is a depth of at least one if $funcRef calls functions
    $$depth++ if($numFuncCalls);
    $$depth += $maxDepth;

    return $numFuncCalls;
} # end sub CountFuncCalls ()

#------------------------------------------------------------------------------#
