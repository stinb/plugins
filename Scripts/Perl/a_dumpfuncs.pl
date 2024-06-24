#!  /usr/bin/perl
#
# Synopsis: Output graphical call tree for list of Ada subprograms (Ada)
#
# Categories: Project Report, Graphic
#
# Languages: Ada
#
# Usage:
sub usage($) 
{
    return << "END_USAGE";
${ \( shift @_ ) }
Usage: dumpfuncs.pl -db database
       -db database   Specify Understand database (required for
                      uperl, inherited from Understand)
END_USAGE
}
#  03-Aug-2001 DLL - updated for Understand::Gui::db()
#
# Bring in the Understand PERL module
# This assumes a PERLLIB environment variable
# See http://www.scitools.com/perl.shtml for setup instructions
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
if ( $db->language() !~ "Ada" ) {
    die "This script is designed for Ada only\n";
}

# Setup a list of functions to dump pictures of
# To add functions, just copy and change the
# function name. Using 1 as the value makes the
# if statement checking them work right below.
# To handle overloaded functions use the full Ada name.

#PERL hash array of Ada names to dump pictures of
#Modify list to functions/procedures that you want to draw 

my %listoffuncs = ();
$listoffuncs{"S0_Main"} = 1;
$listoffuncs{"Stcaif_Application_Interface.Run_Eighty_Hz_Subprograms"} = 1;
$listoffuncs{"queue_package.enqueue"} = 1;

# Loop through every function in the database looking
# for ones in the list in listoffuncs

# Get a list of every function and procedure. 
# See http://www.scitools.com/manuals/latest/html/understand_api/
# to get a full listing of all the kinds of entities you 
# can find with the $db->ents function.

foreach my $func ($db->ents("Function,Procedure")) {
    if ($listoffuncs{$func->longname()}) {
	my $file = "call_" . $func->name() . ".png";
	print $func->longname()," -> ",$file,"\n";

	# This is the line that does the cool stuff!
	# See the Understand Perl module docs for information on options for graph layout
	$func->draw("invocation",$file);
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

