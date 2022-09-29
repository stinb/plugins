
# Walk through all Lexemes in a project
#
# Synopsis: Lists all functions
#
#   Lists all functions.
#   Requires an existing Understand database
#
# Language: All
#
#  For the latest Understand perl API documentation, see
#      http://www.scitools.com/perl.html
#
#  01-Apr-2013 KG
#
# Usage:
sub usage($) {
    return shift(@_) . <<"END_USAGE";
Usage: $0 -db database
  -db database      Specify Understand database (required for
                    uperl, inherited from Understand)
END_USAGE
}

use Understand;
use Getopt::Long;
use strict;

my $dbPath;
my $help;
GetOptions(
     "db=s" => \$dbPath,
     "help" => \$help,
          );

# help message
die usage("") if ($help);

# open the database
my $db=openDatabase($dbPath);

#code body*******************************************************************

my @ents = $db->ents("file ~unknown ~unresolved");
foreach my $file (sort {lc($a->longname()) cmp lc($b->longname());} @ents){
  next if $file->library();
  print $ent->longname . "\n";
  my $lexer = $file->lexer;
  next unless $lexer;
  my $lexeme = $lexer->first();
  while ($lexeme){
  
  }continue{
    $lexeme = $lexeme->next;
  }
}

#end body********************************************************************
closeDatabase($db);


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
    $db->close() if $dbPath;
}

# Pass in percent complete (decimal value) to update a progress bar in the GUI or command line
sub progress{
  my $percent = shift;
  if(Understand::Gui::db()){
    Understand::Gui::progress_bar($percent);
    Understand::Gui::yield();
  }else{
    print "Progress: ".int($percent*100)."%\r";
  }
}
