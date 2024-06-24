#
# Sample Understand perl API program 
#
# Synopsis: Report Halstead Complexity for each function/file in project
#
# Language: Ada, C/C++, Java, Fortran
#
# Usage:

sub usage ($)
{
    return << "END_USAGE";
${ \( shift @_ ) }
Usage: halstead.pl -db database  [-comma] 
  -db database    Specify Understand database (required for
	          uperl, inherited from Understand)
  -comma          (optional) Specify for comma delimited format which can
                   be used in a spreadsheet.
END_USAGE
}

# Description:
#  Report Halstead Complexity for each function/file in project. 
#  See http://www.sei.cmu.edu/activities/str/descriptions/halstead_body.html
#  Any identifier, constant, or string is considered an operand. Any operator
#  or keyword or punctuation (except for )}]) is considered an operator.
#  Specify the -comma option to report in a comma-delimited format for 
#  importing into a spreadsheet.
#  Requires an existing Understand database
#
#  For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl/
#  Refer to the documenation for details on using the perl API 
#  with Understand and for details on all Understand perl API calls.
# 
# 26-Mar-2001 TLB
# 04-Aug-2001 DLL - updated for Understand::Gui::db()
#
use Understand;
use Getopt::Long;
use strict;

my $dbPath;
my $comma;
my $help;
GetOptions(
	   "db=s" => \$dbPath,
           "comma" => \$comma,
	   "help" => \$help,
           );
                  
# help message
die usage("") if ($help);

# open the database 
my $db=openDatabase($dbPath);

# verify supported language
my $dblanguage = $db->language();

if ($dblanguage !~ /ada|c|java|fortran|c#/i) {
    closeDatabase($db);
    die "$dblanguage is currently unsupported";
}

my %project_metric = ();
my $sep;

# set the column separator to be used
if ($comma)    
   { $sep = ",";}
else
   { $sep = "\t";}

# print the column headers
print "Entity Name".$sep."n1".$sep."n2".$sep."N1".$sep."N2".$sep."Len.".
    $sep."Voc.".$sep."Vol.".$sep."Dif.".$sep."Eff.\n\n";

# report by file
foreach my $file (sort {$a->longname() cmp $b->longname();} $db->ents("file ~unknown ~unresolved"))
    {
    # skip standard library functions
    next if ($file->library() =~ /standard/i);

    my %file_metric = ();
    my $lexer = $file->lexer();
    die("Could not open lexer for ".$file->longname().\nAnalyzing Changed Files may fix this."\n ") unless $lexer;
    my $lexeme = $lexer->first();
   
    my @funcs = getFuncs($db,$file,$lexer);
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
	
	my $V = $n ? int ($N * ((log $n)/(log 2))) : 0;
	my $D = $n2 ? int (($n1 / 2) * ($N2 / $n2)) : 0;
	my $E = $D * $V;
	# print the metrics for this function
	print join ($sep, $func->name(), $n1, $n2, $N1, $N2, $N, $n, $V, $D, $E),"\n";
	
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
    if (@funcs)
    {
	print "File Totals:\n";
	print join ($sep, $file->name(), $file_metric{n1}, $file_metric{n2}, $file_metric{N1}, $file_metric{N2}, $file_metric{N}, $file_metric{n}, $file_metric{V}, $file_metric{D}, $file_metric{E}), "\n\n";
	
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
print join ($sep, $project_metric{n1}, $project_metric{n2}, $project_metric{N1}, $project_metric{N2}, $project_metric{N}, $project_metric{n}, $project_metric{V}, $project_metric{D}, $project_metric{E}), "\n";

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
    } elsif ($filelanguage =~ /java|c#/i) {
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
    } elsif ($entlanguage =~ /c|fortran|java|c#/i) {
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
