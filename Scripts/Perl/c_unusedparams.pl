#
#unusedparams.pl - find unused parameters in an Understand project
#
# Synopsis: Finds unused function parameters (C/C++)
#
# Categories: Project Report, Coding Standards
#
# Languages: C
#
# Finds function parameters that are unused by the
# functions they are defined for.
#
# Usage:
sub usage($) 
{
    return <<"END_USAGE";
${ \( shift @_ ) }
Usage: unusedparams.pl -db database
-db database     Specify Understand database (required for
                 uperl, inherited from Understand)
END_USAGE
}


# Designed to be used from the Options->Run A Command
# window of Understand, in that its output can be 
# double clicked on to visit instances of unused parameters
#
# It can also be used from the command line:
#
#   uperl unusedparams.pl -db project.udc
#
# Known problems: Doesn't yet handle (void) as a parameter
# 
# Author: Ken Nelson
#         http://www.scitools.com/

# Get the Understand Perl module
# Note that you can also specify the full path if
# your PERLLIB environment variable isn't set
# See http://www.scitools.com/perl/
#
# 06-Aug-2001 DLL - updated for Understand::Gui::db()

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

# check language
# language-specific
if ( $db->language() !~ "C" ) {
    die "This script is designed for C/C++ only\n";
}
 

# Now get a list of all functions that are properly defined/declared
# in the projects source code

my @funcs = $db->ents("function ~unresolved ~unknown");

# Loop through each function (first sorting) and check
# each parameter for use/set references

foreach my $func (sort {$a->longname() cmp $b->longname();} @funcs)
{
   my $numfound=0;
   
   # For each parameter defined for the current function
   
   foreach my $param ($func->ents("Define","Parameter"))
   {
   
     # check to see if current parameter is Set or Used
     
     if (! $param->refs("SetBy,UseBy"))
     {
       if ($numfound == 0)
       {
        
        # Print out information about the functions definition
        
        foreach my $def ($func->refs("DefineIn"))
        {   
          print $func->longname() . "()\n\tFile: " . $def->file()->longname() . " Line: " . $def->line() . "\n";
          $numfound++;
        }
        print "\t" . $param->name();

       }
        else
       {
         print "," . $param->name();
       }
     }
   }
   if ($numfound > 0)
   { 
           print " unused\n";
   }

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


