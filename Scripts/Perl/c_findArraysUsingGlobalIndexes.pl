#
#  Synopsis: Lists functions and external functions/variables used within them.
#  
# Categories: Project Report
#
# Languages: C
#
# Usage:
sub usage ($)
{
    return << "END_USAGE";
${ \( shift @_ ) }
Usage: c_findArraysUsingGlobalIndexs.pl -db database
  -db database      Specify Understand database (required for
	            uperl, inherited from Understand)
END_USAGE
}

#  For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl.html
# 


use Understand;
use Getopt::Long;

my $dbPath;
GetOptions("db=s" => \$dbPath);

 # open the database 
 my $db=openDatabase($dbPath);


 #open each file and a lexer stream for that file
 
 foreach my $file ($db->ents("~unresolved ~unknown File"))
 {
  my $lexer = $file->lexer();
  my $inArray = 0;
 
  # scan for [ ] blocks.
  foreach $lexeme ($lexer->lexemes())
  {
    
    if ($lexeme->text() eq "[")
    {
       $inArray++; #push an array on the stack
    }
    
    # check to see if there is an entity associated with this lexeme, if so, check if it is global.
    if ($lexeme->ent())
    {
      if ($inArray > 0 && $lexeme->ent()->kindname() eq "Global Object" )
      {
         print "\t" . $lexeme->ent()->longname() . " File: " . $file->name() . " Line: " . $lexeme->line_begin() . "\n";
      }
    }

    # check for end of array index.
    if ($lexeme->text() eq "]")
    {
      $inArray--;  # pop an array off stack

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
	my $status = undef;
	die usage("Error, database not specified\n\n") unless ($dbPath);
	($db,$status)=Understand::open($dbPath);
	die "Error opening database: ",$status,"\n" if $status;
    }
    return($db);
}

