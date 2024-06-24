#
# Sample Understand perl API program 
#
# Synopsis: Lists fullnames of all Ada procedures and functions in a project
#
# Categories: Project Report
#
# Languages: Ada
#
# Usage:
sub usage($) 
{
    return <<"END_USAGE";
${ \( shift @_ ) }
Usage: subprogram_list.pl -db database [-m]
  -db database  Specify Understand database (required for
                uperl, inherited from Understand)
  -m            (optional) Specify for 'minimal' output of subprogram 
                name only.  By default, declaration references for the 
                subprogram are included for double-click visiting 
                within Understand
END_USAGE
}

#Description:
#   Lists longnames of all subprograms in a project to STDOUT, sorted alphabetically.
#   Output can be used within Understand to visit declaration references reported on
#   or reference info can be omitted by specify -m option.
#   Standard library entities are ignored
#   An existing Understand for Ada database is required
#
#  For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl/
#  Refer to the documenation for details on using the perl API 
#  with Understand and for details on all Understand perl API calls.
# 
# 17-Jan-2001 TLB
# 05-Aug-2001 DLL - updated for Understand::Gui::db()
#

use Understand;
use Getopt::Long;
use strict;

my $dbPath;
my $minimal;
my $help;
GetOptions(
	   "db=s" => \$dbPath,
	   "m" => \$minimal,
	   "help" => \$help,
          );

# help message
die usage("") if ($help);

# open the database 
my $db=openDatabase($dbPath);

# check language
if ( $db->language() !~ "Ada" ) {
    die "This script is designed for Ada only\n";
}


foreach my $ent (sort {$a->longname() cmp $b->longname();} $db->ents("function ~formal, procedure ~formal"))
{
   if ($ent->library() ne "Standard") 
   {
      print $ent->longname();
      if (!$minimal)
      {
         my @decl = getDeclareinRef($ent);
         if (@decl)
         {
            print "   [File: ".$decl[0]->file()->longname()."  Line: ".$decl[0]->line()."]"
         }
      }
      print "\n";
   }
}

closeDatabase($db);


# subroutines


# get declarein reference of specified entity
# there may be several, so get "best"
sub getDeclareinRef($)
{
   my $ent = shift @_;
   return () unless defined($ent);
   
   my @decl=();   
   my @declOrder = ("declarein ~spec ~body ~instance ~formal ~incomplete ~private ~stub",
                 "spec declarein",
                 "body declarein",
                 "instance declarein",
                 "formal declarein",
                 "incomplete declarein",
                 "private declarein",
                 "stub declarein");
               
   foreach my $type (@declOrder)
   {
      @decl = $ent->refs($type);
      if (@decl) { last;  }
   }
   return @decl;
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
