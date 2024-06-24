# c_misra_maint.pl
#
# Synopsis: Reports MISRA maintainability metrics.
#
# Categories: Project Metrics
#
# Languages: C
#
# MISRA maintainability metrics.
#
# - Average statement size
# - Component length
# - Number of operands and operators
#
use Understand;
use Getopt::Long;
use strict;

# defined threshold values for metric reporting
use constant MAX_AVG_STATEMENT_SIZE => 9;
use constant MAX_COMPONENT_LENGTH   => 250;

use constant MAX_n1                 => 35;
use constant MAX_n2                 => 50;
use constant MAX_N1                 => 140;

#------------------------------------------------------------------------------#

my $dbPath;
GetOptions("db=s" => \$dbPath);

# open the Understand database
my $db = OpenDatabase($dbPath);
die unless($db);

# verify supported language
my $language = $db->language();
if ($language !~ /ada|c|java/i) {
    closeDatabase($db);
    die "$language is currently unsupported";
}

# result arrays
my @avgStatementSize;
my @componentLength;
my @numDistinctOperands_n1;
my @numDistinctOperators_n2;
my @numOperandOccurrences_N1;

# look at each file in the project
foreach my $fileEnt ($db->ents("file ~unknown ~unresolved")) {
    # skip standard library functions
    next if ($fileEnt->library() =~ /standard/i);
    
    # grab the lexer for the file
    my ($lexer,$status) = $fileEnt->lexer();
    die $status if($status);
    
    # get all functions defined in $fileEnt
    my @functions = GetFunctions($db,$fileEnt,$lexer);    
    foreach my $function (@functions) {
       	my $startRef = GetDeclRef($function);
        my $endRef = $function->refs("end");
        unless($startRef and $endRef) {
            print "no endref\n";
            next;
        } # end unless($startRef and $endRef)

        # retrieve the Halstead values for each function
        my ($n1,$n2,$N1,$N2) = GetHalsteadBaseMetrics($lexer,
                                                        $startRef->line(),
                                                        $endRef->line());
        my $numStatements = $function->metric("CountStmtExe");
        my $Len = ($N1 + $N2);

        # check average statement size
        if($numStatements && (int($Len/$numStatements) > MAX_AVG_STATEMENT_SIZE)) {
            my $badFunc = {};
            $badFunc->{"fileName"} = $fileEnt->name();
            $badFunc->{"funcName"} = $function->name();
            $badFunc->{"value"} = int($Len/$numStatements);
            $badFunc->{"limit"} = MAX_AVG_STATEMENT_SIZE;
            push(@avgStatementSize,$badFunc);
        } # end if(...)
        
        # check component length
        if($Len > MAX_COMPONENT_LENGTH) {
            my $badFunc = {};
            $badFunc->{"fileName"} = $fileEnt->name();
            $badFunc->{"funcName"} = $function->name();
            $badFunc->{"value"} = $Len;
            $badFunc->{"limit"} = MAX_COMPONENT_LENGTH;
            push(@componentLength,$badFunc);
        } # end if($Len > MAX_COMPONENT_LENGTH)
        
        # check number of operands and operators
        if($n1 > MAX_n1) {
            my $badFunc = {};
            $badFunc->{"fileName"} = $fileEnt->name();
            $badFunc->{"funcName"} = $function->name();
            $badFunc->{"value"} = $n1;
            $badFunc->{"limit"} = MAX_n1;
            push(@numDistinctOperands_n1,$badFunc);
        } # end if($n1 > MAX_n1)
        if($n2 > MAX_n2) {
            my $badFunc = {};
            $badFunc->{"fileName"} = $fileEnt->name();
            $badFunc->{"funcName"} = $function->name();
            $badFunc->{"value"} = $n2;
            $badFunc->{"limit"} = MAX_n2;
            push(@numDistinctOperators_n2,$badFunc);
        } # end if($n2 > MAX_n2)
        if($N1 > MAX_N1) {
            my $badFunc = {};
            $badFunc->{"fileName"} = $fileEnt->name();
            $badFunc->{"funcName"} = $function->name();
            $badFunc->{"value"} = $N1;
            $badFunc->{"limit"} = MAX_N1;
            push(@numOperandOccurrences_N1,$badFunc);
        } # end if($N1 > MAX_N1)
    } # end foreach my $function (@functions)    
} # end foreach my $fileEnt ($db->ents("file ~unknown ~unresolved"))

# display results
if(@avgStatementSize) {
    print("Functions exceeding Average Statement Size [".MAX_AVG_STATEMENT_SIZE."]\n");
    foreach my $badFunc (@avgStatementSize) {
        print("\t".
              $badFunc->{"fileName"}.", ".
              $badFunc->{"funcName"}.", ".
              $badFunc->{"value"}.", ".
              $badFunc->{"limit"}.
              "\n"
             );
    } # end foreach my $badFunc (@componentLength)
    print("\n");
} # end if(@avgStatementSize)
if(@componentLength) {
    print("Functions exceeding Component Length [".MAX_COMPONENT_LENGTH."]\n");
    foreach my $badFunc (@componentLength) {
        print("\t".
              $badFunc->{"fileName"}.", ".
              $badFunc->{"funcName"}.", ".
              $badFunc->{"value"}.", ".
              $badFunc->{"limit"}.
              "\n"
             );
    } # end foreach my $badFunc (@componentLength)
    print("\n");
} # end if(@componentLength)
if(@numDistinctOperands_n1) {
    print("Functions exceeding n1 [".MAX_n1."]\n");
    foreach my $badFunc (@numDistinctOperands_n1) {
        print("\t".
              $badFunc->{"fileName"}.", ".
              $badFunc->{"funcName"}.", ".
              $badFunc->{"value"}.", ".
              $badFunc->{"limit"}.
              "\n"
             );
    } # end foreach my $badFunc (@numDistinctOperands_n1)
    print("\n");
} # end if(@numDistinctOperands_n1)
if(@numDistinctOperators_n2) {
    print("Functions exceeding n2 [".MAX_n2."]\n");
    foreach my $badFunc (@numDistinctOperators_n2) {
        print("\t".
              $badFunc->{"fileName"}.", ".
              $badFunc->{"funcName"}.", ".
              $badFunc->{"value"}.", ".
              $badFunc->{"limit"}.
              "\n"
             );
    } # end foreach my $badFunc (@numDistinctOperators_n2)
    print("\n");
} # end if(@numDistinctOperators_n2)
if(@numOperandOccurrences_N1) {
    print("Functions exeeding N1 [".MAX_N1."]\n");
    foreach my $badFunc (@numOperandOccurrences_N1) {
        print("\t".
              $badFunc->{"fileName"}.", ".
              $badFunc->{"funcName"}.", ".
              $badFunc->{"value"}.", ".
              $badFunc->{"limit"}.
              "\n"
             );
    } # end foreach my $badFunc (@numOperandOccurrences_N1)
    print("\n");
} # end if(@numOperandOccurances_N1)
                     
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
        my $ent = $lexeme->entity() if $lexeme->ref();
		
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

sub GetHalsteadBaseMetrics {
    my ($lexer,$startLine,$endLine) = @_;
    my $n1=0;
    my $n2=0;
    my $N1=0;
    my $N2=0;
    
    my %n1 = ();
    my %n2 = ();
    
    foreach my $lexeme ($lexer->lexemes($startLine,$endLine)) {
        if(($lexeme->token eq "Operator") ||
                ($lexeme->token eq "Keyword") ||
                ($lexeme->token eq "Punctuation")) {  
            if($lexeme->text() !~ /[)}\]]/) {
                $n1{$lexeme->text()} = 1;
                $N1++;
            } # end if($lexeme->text() !~ /[)}\]]/)
        }elsif(($lexeme->token eq "Identifier") ||
                ($lexeme->token eq "Literal") || ($lexeme->token eq "String")){
            $n2{$lexeme->text()} = 1;
            $N2++;
        } # end if(...)
    } # end foreach my $lexeme ($lexer->lexemes($startLine,$endLine))
    
    $n1 = scalar(keys(%n1));
    $n2 = scalar(keys(%n2));  
    return ($n1,$n2,$N1,$N2);
} # end sub GetHalsteadBaseMetrics ()

#------------------------------------------------------------------------------#
sub GetStatementCount {
    my ($lexer,$startLine,$endLine) = @_;
    my $numStatements = 0;

    my @curlyStack;
    my @parenStack;
    foreach my $lexeme ($lexer->lexemes($startLine,$endLine)) {
        # don't start looking for statements until we've found the body of
        # the function
        next if(!@curlyStack && ($lexeme->token() ne "Punctuation") &&
                ($lexeme->text() ne "{"));
        
        # look for statements
        if(($lexeme->token() eq "Punctuation") && ($lexeme->text() eq "{")) {
            # any set of curly-braces (except the function body) is considered
            # a single statement
            $numStatements++ if(@curlyStack);
            push(@curlyStack,"{");
        }elsif(($lexeme->token() eq "Punctuation") && ($lexeme->text() eq "}")) {
            pop(@curlyStack);
        }elsif(($lexeme->token() eq "Punctuation") && ($lexeme->text() eq "(")) {
            push(@parenStack,"(");
        }elsif(($lexeme->token() eq "Punctuation") && ($lexeme->text() eq ")")) {
            pop(@parenStack);
        }elsif(@parenStack) {
            # ignore what's contained within parenthesis
            next;
        }elsif(($lexeme->token() eq "Punctuation") && ($lexeme->text() eq ";")) {
            $numStatements++;
        } # end if(...)
    } # end foreach my $lexeme ($lexer->lexemes($startLine,$endLine))
    return $numStatements;
} # end sub GetStatementCount ()

#------------------------------------------------------------------------------#
