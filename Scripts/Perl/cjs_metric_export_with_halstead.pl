# Synopsis: Export all metrics to a csv file, same as Projects|Export Metrics in the GUI with Halstead metrics
#
# Language: C/C++, Java, C#
#
# Usage:
sub usage ($)
{
    return << "END_USAGE";
${ \(shift @_) }
Usage: cjs_metric_export_with_halstead.pl -db database -f name
       -db database      Specify Understand database (required for
                         uperl, inherited from Understand)
       -f|file name      name of the file to export (*.csv)
END_USAGE
}

use Understand;
use Getopt::Long;
use strict;

my $dbPath;
my $help;
my %metrics;
my %metricList;
my %file_metric;
my $exportFile;
GetOptions(
	   "db=s" => \$dbPath,
	   "help" => \$help,
     "f|file=s" => \$exportFile,
          );

# help message
die usage("") if ($help);

# export file
die usage("Please specify the file to export metrics to\n") if (! $exportFile);

# open the database
my $db=openDatabase($dbPath);

# check language
if ( $db->language() !~ "C|Java|c#" ) {
    die "This script is designed for C and Java only\n";
}

my $functions = "Function ~unknown ~unresolved,method ~unknown ~unresolved";
my $classes = "Class ~unknown ~unresolved ,Struct ~unknown ~unresolved,Union ~unknown ~unresolved";
my $codefiles = "Code File ~unknown ~unresolved";
my $headerFiles = "Header File ~unknown ~unresolved";


my @ents = $db->ents($functions.",".$classes.",".$codefiles.",".$headerFiles);

    foreach my $ent (@ents){
      next if $ent->library;
        foreach my $metric($ent->metrics()){ 
            $metrics{$ent->id()}{$metric}=$ent->metric($metric);
            $metricList{$metric} = 1;
	}
}
    
printMetrics($exportFile);
print "Metrics exported to $exportFile\n";

closeDatabase($db);


# subroutines
# print the metrics to the specified file
sub printMetrics()
{
    my $filename = shift;
    open (FILEHANDLE, ">".$filename) || die "Can't create $filename: $! \n";
    
    #print metric header
    print FILEHANDLE "Kind,Name";
    for my $metric(sort keys %metricList) {
        print FILEHANDLE ",$metric";
    }
    print FILEHANDLE ",n1,n2,N1,N2,Len,Voc,Vol,Dif,Eff";
    print FILEHANDLE "\n";
    
    #Print Functions
    foreach my $ent (sort {lc($a->longname()) cmp lc($b->longname());} $db->ents($functions)){
        next if $ent->library;
        print FILEHANDLE "Function,\"".$ent->longname()."\"";
        for my $metric(sort keys %metricList) {
            my $metValue = $metrics{$ent->id}{$metric};
            print FILEHANDLE $metValue?",".$metValue:",0"
        }
        print FILEHANDLE halsteadFunc($ent);
        print FILEHANDLE "\n";
    }
    
    #Print Classes
    foreach my $ent (sort {lc($a->longname()) cmp lc($b->longname());} $db->ents($classes)){
        next if $ent->library;
        my $kindname = "Class";
        $kindname = "Struct" if $ent->kind->check("Struct");
        $kindname = "Union" if $ent->kind->check("Union");
        print FILEHANDLE "$kindname,\"".$ent->longname()."\"";
        for my $metric(sort keys %metricList) {
            my $metValue = $metrics{$ent->id}{$metric};
            print FILEHANDLE $metValue?",".$metValue:",0"
        }
        print FILEHANDLE ",0,0,0,0,0,0,0,0,0";
        print FILEHANDLE "\n";
    }
    
    #Print code files
    foreach my $ent (sort {lc($a->name()) cmp lc($b->name());} $db->ents($codefiles)){
        next if $ent->library;
        print FILEHANDLE"CodeFile".",".$ent->name();
        for my $metric(sort keys %metricList) {
            my $metValue = $metrics{$ent->id}{$metric};
            print FILEHANDLE $metValue?",".$metValue:",0"
        }
        print FILEHANDLE halsteadFile($ent);
        print FILEHANDLE "\n";
    }
    
    #Print Header Files
    foreach my $ent (sort {lc($a->name()) cmp lc($b->name());} $db->ents($headerFiles)){
        next if $ent->library;
        print FILEHANDLE"HeaderFile".",".$ent->name();
        for my $metric(sort keys %metricList) {
            my $metValue = $metrics{$ent->id}{$metric};
            print FILEHANDLE $metValue?",".$metValue:",0"
        }
        print FILEHANDLE halsteadFile($ent);
        print FILEHANDLE "\n";
    }
    close (FILEHANDLE);
    
}

sub halsteadFunc {
        my $func = shift;
	my $startref = getDeclRef($func);
	my ($endref) = $func->refs("end","",1);
	
	unless ($startref and $endref) {
	    #print "warning: no end reference in ".$func->longname."\n";
            return ",0,0,0,0,0,0,0,0,0";
	}
	my $file = $startref->file;
        my $fileid = $file->id;
        my $lexer = $file->lexer();
	my ($n1, $n2, $N1, $N2) = scanEntity ($lexer,$startref->line(),$endref->line());
	
	# do the calcs
	my ($n, $N) = ($n1 + $n2, $N1 + $N2);
	
	my $V = $n ? int ($N * ((log $n)/(log 2))) : 0;
	my $D = $n2 ? int (($n1 / 2) * ($N2 / $n2)) : 0;
	my $E = $D * $V;

	
	# add them to the file-based metrics
	$file_metric{$fileid}{n1} += $n1;
	$file_metric{$fileid}{n2} += $n2;
	$file_metric{$fileid}{N1} += $N1;
	$file_metric{$fileid}{N2} += $N2;
	$file_metric{$fileid}{N} += $N;
	$file_metric{$fileid}{n} += $n;
	$file_metric{$fileid}{V} += $V;
	$file_metric{$fileid}{D} += $D;
	$file_metric{$fileid}{E} += $E;
        
  	# print the metrics for this function      
        return ",$n1, $n2, $N1, $N2, $N, $n, $V, $D, $E";
}

sub halsteadFile($)
{
    my $file = shift;
    my $fileid = $file->id();
    my $rstring = "";
    my @array = ("$file_metric{$fileid}{n1}", "$file_metric{$fileid}{n2}", "$file_metric{$fileid}{N1}"
            ,"$file_metric{$fileid}{N2}", "$file_metric{$fileid}{N}", "$file_metric{$fileid}{n}"
            ,"$file_metric{$fileid}{V}", "$file_metric{$fileid}{D}", "$file_metric{$fileid}{E}");
    foreach (@array){ $rstring .= $_?",$_":",0";}
    return $rstring;
}

# return declaration ref (based on language) or 0 if unknown
sub getDeclRef 
{
    my ($ent) =@_;
    my $decl;
    return $decl unless defined ($ent);
    
    ($decl) = $ent->refs("definein","",1);
    ($decl) = $ent->refs("declarein","",1) unless ($decl);
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


sub callby_tree {
    my ($level,$entity,$used,$maxlevels,$parents) = @_;

    foreach my $ref ($entity->refs("callby ~inactive","",1)) {
	my $ent = $ref->ent();
	next if exists $$used{$ent->id()};
	$$used{$ent->id()} = 1;
	if (!$maxlevels || $level < $maxlevels) {
	    my $found=0;
	    foreach my $parent (@$parents) {
		$found = 1 if $parent == $ent;
	    }
	    if (!$found) {
		push @$parents,$ent;
		callby_tree($level+1,$ent,$used,$maxlevels,$parents);
		pop @$parents;
	    }
	}
    }
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
    $db->close() if (!Understand::Gui::active());
}

