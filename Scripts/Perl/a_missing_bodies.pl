#
# Synopsis: Report all compilation units that offer subprograms but don't have bodies.
#
# Categories: Project Report, Coding Standards
#
# Languages: Ada
#
# Description:
#   Report all compilation units that do not have implementing bodies.
#   Skips data/type only specs to report only specs that offer subroutines but have no bodies implementing them.
#
#   For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl/
#

sub usage($) {
    print shift,"\n";
    print "Usage: a_missing_bodies.pl\n";
    print "  -db   database    Specify database, if used with uperl.\n";
    print "                    the used line displayed. The default is 1 line.\n";
}


use Understand;
use Getopt::Long;
use strict;


# get options
my ($context,$dbPath,$help,@typeNames,$showObjs,$showTypes,$showUsed);
GetOptions
    (
     "db=s"      => \$dbPath,
     "help"      => \$help,
     ) or die usage("");
die usage("") if ($help);

# open the database
my $db=Understand::Gui::db();
if (!$db) {
    die usage("Error, database not specified") unless $dbPath;
    my $status;
    ($db,$status)=Understand::open($dbPath);
    die print "Error opening database: $status\n" if $status;
}


# check language
my $language = $db->language();
die print "$language is currently unsupported" if ($language !~ /ada/i);
 

#
# check everything that can have a body
#

foreach my $ent ( $db->ents( "package"
                            .",procedure ~abstract"
                            .",function ~abstract"
                            .",task ~object"
                            .",protected ~object") )
 {
   my $has_body = 0;

   if ( $ent->library() !~ /standard/i )
    {
      #
      # check for "stubs" that indicate a subunit is needed ... BTW, if it
      # declares a "stub" then you know that you've found a body and you won't
      # need to look at this entity in the next stage
      #

      foreach my $stub ( $ent->refs("declare stub") )
       {
         $has_body = 1;

         #
         # unless the "stubbed" enitity has a body file, its among the missing
         # ... I don't know how "parameters" sneak into here, but they do
         #

         unless (   $stub->ent()->kind()->longname() =~ /parameter/i
                 || $stub->ent()->refs("declarein body file") )
          {
            print  "missing subunit : "
                  . $stub->ent()->kind()->longname()
                  ." : "
                  .$stub->ent()->longname()
                  ."\n";
          }
       }

      #
      # how we'll check for bodies for everything that has not declared a "stub"
      #

      unless ( $has_body )
       {
         my $kind = $ent->kind()->longname();

         if (   $kind =~ /package/i
             || $kind =~ /function/i
             || $kind =~ /procedure/i )
          {
            if (    $ent->refs("declarein spec file")
                && !($ent->refs("declarein body file")) )
             {
               foreach my $ref ( $ent->refs("declare", "function ~abstract"
                                                      .",procedure ~abstract"
                                                      .",task ~object"
                                                      .",protected ~object") )
                {
                  unless ( $ref->ent()->refs( "instanceof"
                                             .",rename"
                                             .",derivefrom"
                                             .",subtypefrom") )
                   {
         
         	     my $location = $ent->ref("declare");
         	     
                     print  "missing body : "
                           .$ent->kind()->longname()
                           ." : "
                           .$ent->longname()
                           ." File: "
                           .$location->file()->longname()
                           ."\n";
                     last;
                   }
                }
             }
          }
       }
    }
 }
