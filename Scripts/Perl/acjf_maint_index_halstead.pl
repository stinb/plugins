# Synopsis:   Report Halstead and Maintainability Index for each function/file in project
#
# Language: Ada, C/C++, Java
#
# Description:
#  Report Halstead metrics and Maintainability Index for each function/file in project. 
#  Also calculates for entire project.
#  See http://www.sei.cmu.edu/activities/str/descriptions/halstead_body.html
#  Any identifier, constant, or string is considered an operand. Any operator
#  or keyword or punctuation (except for )}]) is considered an operator.
#  Requires an existing Understand database
#
#  For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl/
#  Refer to the documenation for details on using the perl API 
#  with Understand and for details on all Understand perl API calls.

# Usage:
sub usage ($)
{
    return << "END_USAGE";
${ \( shift @_ ) }
Usage: halstead.pl -db database  [-comma] 
  -db database    Specify Understand database (required for
	          uperl, inherited from Understand)
END_USAGE
}
use Understand;
use Getopt::Long;
use strict;

my $dbPath;
my $comma;
my $help;
GetOptions(
	   "db=s" => \$dbPath,
	   "help" => \$help,
           );
                  
                 
# help message
die usage("") if ($help);

# open the database 
my $db=openDatabase($dbPath);

# verify supported language
my $dblanguage = $db->language();
if ($dblanguage !~ /ada|c|java|fortran/i) {
    closeDatabase($db);
    die "$dblanguage is currently unsupported";
}

my %project_metric = ();
my $sep;

my $projComplexitySum = 0;
my $projLineCount = 0;
my $projFuncCount = 0;

# set the column separator to be used
$sep = ",";

# print the column headers
print "Entity Name".$sep."n1".$sep."n2".$sep."N1".$sep."N2".$sep."Len.".
    $sep."Voc.".$sep."Vol.".$sep."Dif.".$sep."Eff.".$sep."MI\n\n";


# report by file
foreach my $file (sort {$a->longname() cmp $b->longname();} $db->ents("file ~unknown ~unresolved"))
    {
    # skip standard library functions
    next if ($file->library() =~ /standard/i);

    print $file->name() . " Functions:\n===============\n";
   
    my %file_metric = ();
    my $lexer = $file->lexer();
	if (!$lexer){
		print "The file " . $file->name() . " can not be located or is an empty file.  Please check that the file exists in the path. \n\n";
		next;
	}
    my $lexeme = $lexer->first();
 	
    my @funcs = getFuncs($db,$file,$lexer);

    my $fileFuncCount = 0;
    my $fileLineCount = 0;
    my $fileComplexitySum = 0;

    foreach my $func ( sort {$a->name() cmp $b->name();} @funcs)
    {
	my $startref = getDeclRef($func);
	my ($endref) = $func->refs("end","",1);
       	

	unless ($startref and $endref) {
	    print "no endref\n";
	    next;
	}
	my ($n1, $n2, $N1, $N2) = scanEntity ($lexer,$startref->line(),$endref->line());
	   
		
	
	# do the calcs
	my ($n, $N) = ($n1 + $n2, $N1 + $N2);
	
	#avoid log of 0 error
        if ($n <= 0)
        {
          $n = 1;
        }
        
	my $V = $n ? int ($N * ((log $n)/(log 2))) : 0;
	my $D = $n2 ? int (($n1 / 2) * ($N2 / $n2)) : 0;
	my $E = $D * $V;
         
        my $avV;

        #avoid log of 0 error
        if ($V == 0)
        {
          $avV = 1;
        }
        else
        {
          $avV = $V;
        }

        my $metricCountLine = $func->metric("CountLine");
        if ($metricCountLine <= 0)
        {
          $metricCountLine = 1;
        }

	#calculate maintainability index
	#per http://www.sei.cmu.edu/str/descriptions/mitmpm.html
	#use the corrected countLine metric from above
	my $MI = 171 - 5.2 * log($avV) - .23 * $func->metric("CyclomaticStrict") -16.2 * log($metricCountLine) + 50 * sin(sqrt(2.4 * $func->metric("RatioCommentToCode")));

        #Sum data for File
	$fileFuncCount ++;
	$fileLineCount  += $func->metric("CountLine");
	$fileComplexitySum += $func->metric("CyclomaticStrict");
	

        # Sum Data for Project
	$projComplexitySum += $func->metric("CyclomaticStrict");
	$projLineCount += $func->metric("CountLine");
	$projFuncCount ++;

	# print the metrics for this function
	print join ($sep, $func->name(), $n1, $n2, $N1, $N2, $N, $n, $V, $D, $E);
	printf(",%.2f", $MI);
	print "\n";

	# add them to the file-based metrics
	$file_metric{n1} += $n1;
	$file_metric{n2} += $n2;
	$file_metric{N1} += $N1;
	$file_metric{N2} += $N2;
	$file_metric{N} += $N;
	$file_metric{n} += $n;
	$file_metric{V} += $V;
	$file_metric{D} += $D;
	$file_metric{E} += $E;
    }
    
    # if this file has functions defined, report totals for the file
    # and add to the project metric totals
    if (@funcs && $fileFuncCount > 0)
    {

        my ($avG, $avV, $avLoc, $perCM);
   
        #calculate average V, make it 1 if 0 to avoid log error.     
        if ($file_metric{V} == 0)
        {
           $avV = 1;
        }
        else
        {
           $avV = $file_metric{V} / $fileFuncCount;
        }  
        
        $avG = $fileComplexitySum / $fileFuncCount;
        $avLoc = $fileLineCount / $fileFuncCount;
        #avoid log error
        if ($avLoc <= 0)
        {
           $avLoc = 1;
        }
        $perCM = $file->metric("RatioCommentToCode");
               
        my $FileMI;
        
	$FileMI =  171-5.2 * log($avV)-.23*$avG-16.2*log($avLoc) + 50 * sin(sqrt(2.4 * $perCM));

	print "===============\nFile Totals:\n";
	print join ($sep, $file->longname(), $file_metric{n1}, $file_metric{n2}, $file_metric{N1}, $file_metric{N2}, $file_metric{N}, $file_metric{n}, $file_metric{V}, $file_metric{D}, $file_metric{E});
	printf(",%.2f",$FileMI);
	print "\n\n";
	
	$project_metric{n1} += $file_metric{n1};
	$project_metric{n2} += $file_metric{n2};
	$project_metric{N1} += $file_metric{N1};
	$project_metric{N2} += $file_metric{N2};
	$project_metric{N} += $file_metric{N};
	$project_metric{n} += $file_metric{n};
	$project_metric{V} += $file_metric{V};
	$project_metric{D} += $file_metric{D};
	$project_metric{E} += $file_metric{E};
    }
    
}

# all done - report totals for project
print "\nProject Totals:\n";

my ($projMI,$avG, $avV, $avLoc, $perCM);

#avoid log of 0 error
if ($project_metric{V} == 0)
{
  $avV = 1;
}
else
{
  $avV   = $project_metric{V} / $projFuncCount;
}

#avoid dividing by zero
if ($projFuncCount == 0)
{
	$avG = $projComplexitySum;
	$avLoc = $projLineCount;
}
else
{
	$avG   = $projComplexitySum/$projFuncCount;
	$avLoc = $projLineCount / $projFuncCount;
}

#avoid log error
if($avLoc <= 0)
{
    $avLoc = 1;
}
$perCM = $db->metric("RatioCommentToCode");

$projMI =  171-5.2 * log($avV)-.23*$avG-16.2*log($avLoc) + 50 * sin(sqrt(2.4 * $perCM));

print "\tn1: " . $project_metric{n1} . "\n";
print "\tn2: " . $project_metric{n2} . "\n";
print "\tN1: " . $project_metric{N1} . "\n";
print "\tN2: " . $project_metric{N2} . "\n";
print "\tN : " . $project_metric{N}  . "\n";
print "\tn : " . $project_metric{n}  . "\n";
print "\tV : " . $project_metric{V}  . "\n";
print "\tD : " . $project_metric{D}  . "\n";
print "\tE: " . $project_metric{E}   . "\n";
printf("\tMI: %.2f\n",$projMI);

closeDatabase($db);


# return array of functions in a file
sub getFuncs {
    my $db = shift;
    my $file = shift;
    my $lexer = shift;
    my $filelanguage = $file->language();   # use language of $file when available
    my @funcs = ();

    my $refkind;
    my $entkind;
    if ($filelanguage =~ /ada/i) {
	$refkind = "declarein body";
	$entkind = "function,procedure";
    } elsif ($filelanguage =~ /java/i) {
		$refkind = "definein";
		$entkind = "method";
    } elsif ($filelanguage =~ /c/i) {
		$refkind = "definein";
		$entkind = "function";
    } elsif ($filelanguage =~ /fortran/i) {
		$refkind = "definein";
		$entkind = "fortran subroutine, fortran function, fortran main program";
	} else {
		return ();
    }

    $lexer = $file->lexer() if !$lexer;
    foreach my $lexeme ($lexer->lexemes()) {
	next if !$lexeme;
	my $ref = $lexeme->ref();
	my $ent = $lexeme->ent() if $ref;
	if ($ref && $ent && $ref->kind->check($refkind) && $ent->kind->check($entkind)) {
	    push @funcs, $ent;
	}
    }

    return @funcs;
}


# return declaration ref (based on language) or 0 if unknown
sub getDeclRef 
{
    my ($ent) =@_;
	my $entlanguage = $ent->language;
    my $decl;
    return $decl unless defined ($ent);
    
    if ($entlanguage =~ /ada/i) {
	($decl) = $ent->refs("declarein body","",1);
    } elsif ($entlanguage =~ /c|fortran|java/i) {
	($decl) = $ent->refs("definein","",1);
	($decl) = $ent->refs("declarein","",1) unless ($decl);
    }
    return $decl;
}


# scan the code in the specified range of lines
# and return the 4 basic operator/operand metrics
sub scanEntity
{
  my ($lexer, $startline, $endline) = @_;
  my $n1=0;
  my $n2=0;
  my $N1=0;
  my $N2=0;
  
  my %n1 = ();
  my %n2 = ();

  foreach my $lexeme ($lexer->lexemes($startline,$endline)) 
  {
     if (($lexeme->token eq "Operator") || ($lexeme->token eq "Keyword") || ($lexeme->token eq "Punctuation"))
     {  
        if ($lexeme->text() !~ /[)}\]]/)
        {
           $n1{$lexeme->text()} = 1;
           $N1++;
        }
     }
     elsif (($lexeme->token eq "Identifier") || ($lexeme->token eq "Literal") || ($lexeme->token eq "String"))
     {
        $n2{$lexeme->text()} = 1;
        $N2++;
     }
  }
  
  $n1 = scalar( keys(%n1));
  $n2 = scalar( keys(%n2));  
   
  return ($n1,$n2,$N1,$N2);
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

