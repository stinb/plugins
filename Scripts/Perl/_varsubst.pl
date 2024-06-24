#
# Understand perl API program: varsubst.pl 
#
# Synopsis: Replace variables with actual values within comments 
#
# Categories: Code Alteration, Coding Standards
#
# Languages: All
#
# Usage:
sub usage($) 
{
    return <<"END_USAGE";
${ \(shift @_ ) }
Usage: varsubst.pl -db database
 -db database   Specify Understand database (required for 
                uperl, inherited from Understand)
END_USAGE
}


# Assumptions: First comment in file is associated with only the file and
#              for the remainder of the file the comment immediately 
#              preceeding a function will be associated with that function 
#
# Description:
#   Modify source files to replace user defined variables within comments
#   with values known by understand
#   
#  For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl/
#  Refer to the documenation for details on using the perl API 
#  with Understand and for details on all Understand perl API calls.
# 
# 23-Jul-2001 DLL
# 05-Aug-2001 DLL - updated for Understand::Gui::db()
#
use strict;
use Understand;
use Getopt::Long;
use Time::Local;
use File::Basename;

# Change this to your preference
my $modifiedFileSuffix=".x";

#get user options
my $dbPath;
my $help;
GetOptions(
	   "dbPath=s" => \$dbPath,
	   "help" => \$help,
           );

die usage("") if $help;


# open the database 
my $db=openDatabase($dbPath);

# get sorted file entities list
my $kind = "File ~unknown ~unresolved";
foreach my $fileEnt (sort{$a->longname() cmp $b->longname();} $db->ents($kind)) {
    
    # print comments that reference the file
    if (!doLexicalStream($db,$fileEnt)) {
	last;
    }
}

# close the database
closeDatabase($db);


# subroutines

sub doLexicalStream()
{
    my ($db,$fileEnt) = @_;
    
    # create lexer object
    my ($lexer,$status) = $fileEnt->lexer();
    if ($status) {
	print "file $status, cannot examine data\n";
	return(0);
    }


    print "Handling file: ".$fileEnt->longname(), "\n";

    # token stack for entire file
    my @tokens=();
    my $topComment;
    my $comment;
    my $modified;
    my $newText;

    # loop through the entire lexer stream
    foreach my $lexeme ( $lexer->lexemes() ) {

	# push string onto the stack
	push(@tokens,$lexeme->text()); 

	# remember the last comment token index
	if ( $lexeme->token() eq "Comment" ) {
	    
	    if ( !$topComment ) {
		# handle as a top comment 
		$topComment = scalar(@tokens);
		my ($newText,$status) = updateComments($tokens[$topComment-1], $db, $fileEnt, undef);
		if ( $status ) {
		    $tokens[$topComment-1] = $newText;
		    $modified = 1;
		}
	    }
	    else {
		# just remember where it is
		$comment = scalar(@tokens);
	    }
	}

	# found an identifier for previous comment, if any
	if ( $lexeme->token() eq "Identifier" 
	     && $lexeme->ent()
	     && $lexeme->ent()->kindname() =~ /Function/i ) {

	    my ($newText,$status) = updateComments($tokens[$comment-1], $db, $fileEnt, $lexeme->ent());
	    if ( $status ) {
		$tokens[$comment-1] = $newText;
		$modified = 1;
	    }
	}
    }
    
    # write out the updated file 
    if ( $modified ) {

	# create new filename 
	my $newFile = $fileEnt->longname() . $modifiedFileSuffix;
	my $dirName = dirname($newFile);

	print "File: " . $fileEnt->longname() . " modified, saving changes\n";
	print "   in " . $newFile . "\n";
	
	if ( !$newFile || ! -w $dirName) {
	    die "cannot write into directory $dirName";
	}


	open(OUTPUT, "> $newFile")
	    or die "Couldn't open $newFile for write: $!\n";
	foreach my $token (@tokens) {
	    print OUTPUT $token;
	}
	close(OUTPUT);
    }
    
    return (1);
}

sub updateComments()
{
    my ($comment,$db,$fileEnt,$ent) = @_;
    my $modifiedComment = $comment;
    my $status;
    
    if ( !$ent ) {
	# update top-level comment 
	my $undFilename = $fileEnt->longname();
	my $undFuncsInFile = getFuncsInFile($db,$fileEnt);
	my $undFileModDate = getFileModDate($undFilename);

	#print "\$undFilename = $undFilename\n";
	#print "\$undFuncsInFile = $undFuncsInFile\n";
	#print "\$undFileModDate = $undFileModDate\n";
	#print "\$comment = $comment\n";

	# modify any in comments with recognized special variables 
	$modifiedComment =~ s/\$\$UndFilename/$undFilename/g;
	$modifiedComment =~ s/\$\$UndFuncsInFile/$undFuncsInFile/g if $undFuncsInFile;
	$modifiedComment =~ s/\$\$UndFileModDate/$undFileModDate/g if $undFileModDate;
    }
    else {
	# update function-level comments
	my $undFuncName = getFuncName($db,$ent);
	my $undFuncParams = getFuncParams($db,$ent);
	my $undFuncType = getFuncType($db,$ent);
	
	#print "\$undFuncName = $undFuncName\n";
	#print "\$undFuncParams = $undFuncParams\n";
	#print "\$undFuncType = $undFuncType\n";
	#print "\$comment = $comment\n";

	# modify any in comments with recognized special variables 
	$modifiedComment =~ s/\$\$UndFuncName/$undFuncName/g if $undFuncName;
	$modifiedComment =~ s/\$\$UndFuncParams/$undFuncParams/g if $undFuncParams;
	$modifiedComment =~ s/\$\$UndFuncType/$undFuncType/g if $undFuncType;
    }
    
    #return modified comment to caller
    if ( $modifiedComment ne $comment ) {
	$status = 1;
    }

    return($modifiedComment,$status);
}


sub getFuncsInFile()
{
    my ($db,$fileEnt) = @_;

    my $funcs=""; 
    my @functionRefs =$fileEnt->refs("Definedin","Function ~unresolved,Procedure ~unresolved",1);
    foreach my $ref (sort{$a->ent()->longname() cmp $b->ent()->longname();} @functionRefs) {
	if ( $funcs ) {
	    $funcs = $funcs . ", ";
	}
	$funcs = $funcs . $ref->ent()->longname() . "()";
     }
    
    return($funcs);
}

sub getFuncName()
{
    my ($db,$ent) = @_;

    #  bail if not defined or no name
    if ( !$ent || !$ent->name() ) {
	return undef;
    }

    # return it's name
    return($ent->name());
}


    
sub getFuncParams()
{
    my ($db,$ent) = @_;
    
    # bail if not defined
    if ( !$ent ) {
	return undef;
    }

    my $parameters="";
    
    my @parameterRefs=$ent->refs("Define","Parameter",1);
				 
    foreach my $ref (sort{$a->ent()->longname() cmp $b->ent()->longname();} @parameterRefs) {
	if ( $parameters ) {
	    $parameters = $parameters . ", ";
	}
	my $name = $ref->ent()->longname();
	my $type = $ref->ent()->type();
	$parameters = "$parameters $type $name";
    }

    return($parameters);
}

sub getFuncType()
{
    my ($db,$ent) = @_;

    #  bail if not defined or no type
    if ( !$ent || !$ent->type() ) {
	return undef;
    }

    # return it's type
    return($ent->type());
}




sub getFileModDate()
{
    my ($file) = @_;

    my ($dev,$ino,$mode,$nlink,$uid,$gid,$rdev,$size,
	$atime,$mtime,$ctime,$blksize,$blocks) = stat $file;

    return(localtime($mtime));
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

