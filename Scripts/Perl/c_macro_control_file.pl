
# Sample Understand PERL API Script
#
# Synopsis: Create a dynamic file that controls project macro settings and lists all project macros.
#
#   After modifing the script, add it as a macro in Project|Configure|Macro tab using @
#       e.g. @c:\project\macroList.txt
#
# Requires an existing Understand for C++ database
#
# Categories: Code Alteration
# Languages: C
#
# Usage:
sub usage($) {
    return shift(@_) . <<"END_USAGE";
    Usage: $0 -db database
    -db database      Specify Understand database (required for
                      uperl, inherited from Understand)
    -f filename       Specify the filename to output the macros to.
END_USAGE
}
#
#  For the latest Understand perl API documentation, see
#      http://www.scitools.com/perl.html
#
#  4-May-2007 KG

use Understand;
use Getopt::Long;
use strict;

my $dbPath;
my $help;
my $file;
GetOptions(
	   "db=s" => \$dbPath,
	   "help" => \$help,
           "f=s"  => \$file,
          );

# help message
die usage("") if ($help);

# open the database
my $db=openDatabase($dbPath);
my $dbname = $db->name();

# check language
if ( $db->language() !~ "C|Java" ) {
    die "This script is designed for C and Java only\n";
}
if (! $file){
    die "Please provide filename to export macros to\n";
}


#code body*******************************************************************

 my @macros = $db->ents("macro");
  open (MYFILE, '>'.$file);
  print MYFILE file_header();
  foreach my $macro(sort {lc($a->name()) cmp lc($b->name());}@macros){
    if ($macro->ref("definein")){
        print "Skipping ".$macro->name." - defined in code\n";
    }
    elsif ($macro->kind->check("unknown")){
        print MYFILE "#".$macro->name."\n";
    }elsif($macro->value){
        print MYFILE $macro->name."=".$macro->value."\n";
    }else{
        print MYFILE $macro->name."\n";
    }
  }
  close (MYFILE);
  print ("Macros exported to $file\n");
  print("Modify file and add to the the Project|Configure|Macro tab using \@.\n");
  print ("  \@$file\n");
  print ("Re-analyze the project after modifing this file\n");
#end body********************************************************************
closeDatabase($db);


# subroutines

sub file_header() {
    return <<"END_USAGE";
#*******************************************************************************    
#  This file contains macro definitions for $dbname.
#
#  To leave the Macro undefined, leave the pound sign(#) in front of it
#       #macroA
#
#  To set the macro as defined, remove the pound sign(#)
#       macroA
#
#  To assign a value to the macro, remove the pound sign and append the value as
#  follows, both examples assign the value foo to macroA
#       macroA=foo
#       macroA foo
#
#After modifing the script, add it to the Project|Configure|Macro tab using \@
#       \@$file
#
#*******************************************************************************
#
END_USAGE
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

