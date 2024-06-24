#
# Synopsis: Report entities that are defined more than once
#
# Categories: Project Report, Coding Standards
#
# Languages: All
#
# Usage:
sub usage($) 
{
    return <<"END_USAGE";
${ \( shift @_ ) }
Usage: findduplicates.pl -db database
-db database     Specify Understand database (required for
                 uperl, inherited from Understand)
END_USAGE
}


#  For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl/
#  Refer to the documentation for details on using the perl API 
#  with Understand and for details on all Understand perl API calls.
#
# 3 June 2003 KN

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

# variables to save last entity information for duplicate comparison purposes.
 
my $lastentname = "";
my $lastkind = "";
my $lastent = "";
my $firstdup = 1;

# start a loop through entities, sorting the entities first on kind then on longname.

foreach my $ent (
    sort {$a->kindname().$a->longname() cmp $b->kindname() . $b->longname();}
    $db->ents("~unnamed type, object ~local ~member, ~parameter ~object ~type"))
{
   # check if name matches last one.
   if ( ($ent->longname() eq $lastentname) && ($ent->kindname() eq $lastkind))
    {
    # when we find a duplicate the current entity is actually the second of the
    # match, so print out the saved first one. One time.
    if ($firstdup)
    {
     # get the definition 
      my @def = $lastent->refs ("Definein");
      print "\n";
      print ($lastent->kindname() . "  " . $lastent->longname()); 
     if (@def)
      {   printf ("\t(File: %s Line: %d)",
               $def[0]->file()->longname(),
               $def[0]->line() );
      }
      print ("\n");  
      
      $firstdup = 0;
    }
    
    #  print $ent->longname,"/" . $ent->kindname() . "... " . $lastent . "/" . $lastkind;

     # get the definition 
      my @def = $ent->refs ("Definein");
      print ($ent->kindname() . "  " . $ent->longname()); 
     if (@def)
      {   printf ("\t(File: %s Line: %d)",
               $def[0]->file()->longname(),
               $def[0]->line() );
      }
      print ("\n");
   }
   else
   {
    # no match, so reset the flag indicating a first duplicate needs to be printed
     $firstdup = 1;
   }
   
   $lastent = $ent;
   $lastentname = $ent->longname();
   $lastkind = $ent->kindname();
}

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
    $db->close() if ($dbPath);
}


