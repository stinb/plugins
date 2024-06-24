#
# Sample Understand PERL API Script
#
# Synopsis: Lists all global variables and where they are defined (C/C++)
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
Usage: globals -db database
  -db database    Specify Understand database (required for
	          uperl, inherited from Understand)
END_USAGE
}

#  For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl.html
# 
#  22-Nov-2000 DP
#  04-Aug-2001 DLL - updated for Understand::Gui::db()


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
if ( $db->language() !~ "C" ) {
    die "This script is designed for C only\n";
}

my @files = sort {lc($a->longname()) cmp lc($b->longname())} $db->ents("code file");
foreach my $file (@files) {
    #if ($file->longname() !~ /.*(test|sample).*/) {
	my @objs = $file->ents("define","object");
	if (@objs) {
	    print $file->longname(),":\n";
	    foreach my $obj (@objs) {
		print "   ",$obj->longname(),"\n";
	    }
	}
    #}
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

