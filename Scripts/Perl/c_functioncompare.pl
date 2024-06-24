
# Sample Understand PERL API Script
#
# Synopsis: compares functions between two databases. (C/C++)
#
#   Requires two existing Understand for C++ databases
#
# Categories: Project Report, Comparison
#
# Languages: C
#
# Usage:
sub usage($)
{
    return << "END_USAGE";
${ \( shift @_ ) }
Usage: c_functioncompare.pl -db database
 -db database      Specify Understand database (required for uperl, inherited from Understand)
 -prev database          Previous database
 -added                  Also shows added functions
 -deleted                Also shows deleted functions
 -notchanged             Also shows functions that were not changed
 -refresh                Refreshes the databases before opening and comparing
 -short                  The functions longname is replaced by its shortname
 -comments               Include associated comments in comparison
 -c or -ignorecomments   Ignore comments in comparison
 -w or -ignorewhitespace Ignore whitespace in comparison
 -a or -ignorecase       Ignore capitalization in comparion

END_USAGE
}

#  For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl.html
# 
#  14-Dec-2000 KN
#  05-Aug-2001 DLL - updated for Understand::Gui::db()
#  09-Dec-2013 KG - Updated to handle new command line syntax
#  22-Jan-2014 JEM, KG Add ignore options, correct headers

use Understand;
use Getopt::Long;
use strict;

my $dbPath;
my $prev;
my $notchanged;
my $deleted;
my $added;
my $short;
my $help;
my $refresh;
my $comments;
my $ignoreComments;
my $ignoreWhitespace;
my $ignoreCase;
GetOptions(
	   "db=s" => \$dbPath,
	   "prev=s" => \$prev,
	   "notchanged" => \$notchanged,
	   "deleted" => \$deleted,
	   "added" => \$added,
     "refresh" => \$refresh,
     "short" => \$short,
     "comments" => \$comments,
     "c|ignorecomments" => \$ignoreComments,
     "w|ignorewhitespace" => \$ignoreWhitespace,
     "a|ignorecase" => \$ignoreCase,
	   "help" => \$help,
          );


#Setup appropriate titles for Changed and Non-changed (same) reports
my $func_changed_title = "Functions Changed:";
my $func_same_title    = "Functions Not Changed:";
my $ignoreString;
$ignoreString .= "Comment " if $ignoreComments;
$ignoreString .= "Whitespace " if $ignoreWhitespace;
$ignoreString .= "Case " if $ignoreCase;

if ($ignoreString) {
	$func_changed_title = "Functions Changed (ignoring ${ignoreString}differences):";
	$func_same_title    = "Functions Not Changed (ignoring ${ignoreString}differences):";
}

# help message
die usage("") if ($help);
die usage("database specification required") if (!$dbPath);
die usage("previous database specification required") if (!$prev);

# insist the it not be run from within the GUI
if ( Understand::Gui::active() ) {
    die usage("This script is not designed to be called from the GUI");
}

# make sure db files are readable before continuing on
die "Can't read database " . $dbPath if ( $dbPath && ! -r $dbPath );
die "Can't read database " . $prev if ( $prev && ! -r $prev );

# refresh the databases before opening and comparing
if($refresh)
{
	my $und_cmd;
	$und_cmd = "und -quiet -db '$dbPath' analyze ";
	print "Building Understand database $dbPath\n";
	system("$und_cmd") && die "\ncannot execute understand command";

	$und_cmd = "und -quiet -db '$prev' analyze ";
	print "Building Understand database $prev\n";
	system("$und_cmd") && die "\ncannot execute understand command";
}

# open the database 
my $db=openDatabase($dbPath);

# get sorted file entities list
my $kind = "File ~unknown ~unresolved";
my @funcs = sort {$a->relname() cmp $b->relname();} 
                 (grep {$_->library() ne "Standard"} $db->ents($kind));

print STDOUT "Reading Understand database, $dbPath\n";


my $filter = "TempFilterforDebug";

my $flagset;

$flagset = "after";

my %funcsallA;
my %lineNumber;

foreach my $func (sort {$a->longname() cmp $b->longname();} @funcs)
{
	print ".";	
	my $lexer = $func->lexer();
	my @funcRefs = $func->filerefs("define", "function");	
	foreach my $funcRef (@funcRefs) {
		my $endRef = $funcRef->ent->ref("end");
		my @lexemes = $lexer->lexemes($funcRef->line, $endRef->line);

		my $text;

		if($comments)
		{
			$text .= $funcRef->ent->comments($flagset ,"default");
		}

		foreach my $lexeme (@lexemes) {
      if ($ignoreWhitespace){
        next if $lexeme->token =~ /Whitespace|Newline/;
      }
      if ($ignoreComments){
        next if $lexeme->token =~ /Comment/;
      }
			$text .= $lexeme->text;
		}
    $text = lc($text) if $ignoreCase;
    
		if($filter eq $funcRef->ent->longname())

		{
			print $db->name() . "\n";
			print $func->name() . "\n";
			print $funcRef->ent->uniquename() . "\n";
			print $funcRef->ent->longname() . "\n";
			print $funcRef->ent->type() . "\n";	
			print Understand::Util::checksum($text) . "\n";
			print $text . "\n";	
			print "comments: " . $funcRef->ent->comments($flagset ,"default") . "\n";
			print "\n";
		}

		if($short)
		{
			$funcsallA{$func->name(). "\t" . $funcRef->ent->name() } = Understand::Util::checksum($text);
      $lineNumber{$func->name(). "\t" . $funcRef->ent->name() } = $func->name(). "(".$funcRef->line.")\t" . $funcRef->ent->name();
		}
		else
		{
			$funcsallA{$func->name() . "\t" . $funcRef->ent->longname()} = Understand::Util::checksum($text);
      $lineNumber{$func->name() . "\t" . $funcRef->ent->longname() } = $func->name() . "(".$funcRef->line.")\t" . $funcRef->ent->longname()
		}
	}
}

closeDatabase($db);

# open the database 
my $db2=openDatabaseprev($prev);

# get sorted file entities list
my $kind = "File ~unknown ~unresolved";
my @funcs2 = sort {$a->relname() cmp $b->relname();} 
                 (grep {$_->library() ne "Standard"} $db2->ents($kind));

print STDOUT "\nReading Understand database, $prev\n";

my $flagseta;

$flagseta = "after";

my %funcsallB;


foreach my $funca (sort {$a->longname() cmp $b->longname();} @funcs2)
{
	print ".";	
	my $lexera = $funca->lexer();
	my @funcRefsa = $funca->filerefs("define", "function");	
	foreach my $funcRefa (@funcRefsa) {
		my $endRefa = $funcRefa->ent->ref("end");
		my @lexemesa = $lexera->lexemes($funcRefa->line, $endRefa->line);

		my $texta;

		if($comments)
		{
			$texta .= $funcRefa->ent->comments($flagseta ,"default");
		}

		foreach my $lexemea (@lexemesa) {
      if ($ignoreWhitespace){
        next if $lexemea->token =~ /Whitespace|Newline/;
      }
      if ($ignoreComments){
        next if $lexemea->token =~ /Comment/;
      }
			$texta .= $lexemea->text;
		}
    $texta = lc($texta) if $ignoreCase;

		if($filter eq $funcRefa->ent->longname())

		{
			print $db2->name() . "\n";
			print $funca->name() . "\n";
			print $funcRefa->ent->uniquename() . "\n";
			print $funcRefa->ent->longname()  . "\n";
			print $funcRefa->ent->type() . "\n";	
			print Understand::Util::checksum($texta) . "\n";
			print $texta . "\n";
			print "comments: " . $funcRefa->ent->comments($flagseta ,"default") . "\n";
			print "\n";
		}
		#print $funcRef->ent->longname() . "\n";
		if($short)
		{
			$funcsallB{$funca->name() . "\t" . $funcRefa->ent->name()} = Understand::Util::checksum($texta);
      $lineNumber{$funca->name() . "\t" . $funcRefa->ent->name()} = $funca->name() . "(".$funcRefa->line.")\t" . $funcRefa->ent->name() if ! $lineNumber{$funca->name() . "\t" . $funcRefa->ent->name()} ;
		}
		else
		{
			$funcsallB{$funca->name() . "\t" . $funcRefa->ent->longname()} = Understand::Util::checksum($texta);
      $lineNumber{$funca->name() . "\t" . $funcRefa->ent->longname()} = $funca->name() . "(".$funcRefa->line.")\t" . $funcRefa->ent->longname() if ! $lineNumber{$funca->name() . "\t" . $funcRefa->ent->longname()} ;
		}
	}
}

closeDatabaseprev($db2);

print "\n";

my @funcssame;
my @funcsdiff;
my @funcsdel;
my @funcsadd;

my $counter = 0;

foreach my $funca (sort {lc($a) cmp lc($b);} %funcsallA)
{
	if($funcsallA{$funca} ne "")
	{
		$counter = 0;

		foreach my $funcb (sort {lc($a) cmp lc($b);} %funcsallB)
		{
			if($funcsallB{$funcb} ne "")
			{
				if($funca eq $funcb)
				{
					if($funcsallA{$funca} eq $funcsallB{$funcb})
					{
						$counter = 1;
						push( @funcssame, $funca);
					}
					else
					{
						$counter = 1;
						push( @funcsdiff, $funca);
					}
				}
			}
		}

		if($counter eq "0")
		{
			push(@funcsadd, $funca);
		}
	}
}

foreach my $funcb  (sort {lc($a) cmp lc($b);} %funcsallB)
{
	if($funcsallB{$funcb} ne "")
	{
		$counter = 0;


		foreach my $funca (sort {lc($a) cmp lc($b);} %funcsallA)
		{
			if($funcsallA{$funca} ne "")
			{
				if($funcb eq $funca)
				{
					$counter = 1;
				}
			}
		}
		if($counter eq "0")
		{
			push(@funcsdel, $funcb);
		}
	}
}

my $header = 0;

if($added)
{
	$header = 0;
	foreach my $out (@funcsadd)
	{
		if($header eq "0")
		{
			print "\n" . "Functions Added:\n";
		}
		$header = 1;
		print "  " . $lineNumber{$out} . "\n";
	}
}

if($deleted)
{
	$header = 0;
	foreach my $out (@funcsdel)
	{
		if($header eq "0")
		{
			print "\n" . "Functions Deleted:\n";
		}
		$header = 1;
		print "  " . $lineNumber{$out} . "\n";
	}
}

if($notchanged)
{
	$header = 0;
	foreach my $out (@funcssame)
	{
		if($header eq "0")
		{
			#modified print to accomodate dynamic title
			print "\n" . $func_same_title . "\n";
		}
		$header = 1;
		print "  " . $lineNumber{$out} . "\n";
	}
}

$header = 0;
foreach my $out (@funcsdiff)
{
	if($header eq "0")
	{
		#modified print to accomodate dynamic title
		print "\n" . $func_changed_title . "\n";
	}
	$header = 1;
	print "  " . $lineNumber{$out} . "\n";
}


# subroutines

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

sub openDatabaseprev($)
{
    my ($prev) = @_;
    
    my $db2 = Understand::Gui::db();

    # open database if not already open
    if (!$db2) {
	my $status;
	die usage("Error, database not specified\n\n") unless ($prev);
	($db2,$status)=Understand::open($prev);
	die "Error opening database: ",$status,"\n" if $status;
    }
    return($db2);
}

sub closeDatabaseprev($)
{
    my ($db2)=@_;

    # close database only if we opened it
    $db2->close() if ($dbPath);
}
