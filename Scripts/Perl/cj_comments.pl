
# Sample Understand PERL API Script
#
# Synopsis: Lists all functions and their comments (C/C++, Java)
#
#   Lists all functions and their associated comments. By default
#   uses longest comment, see documentation for $func->comment() for
#   other comment extraction options.
#
#   Requires an existing Understand for C++ or Java database
#
# Categories: Project Report
#
# Languages: C, Java
#
# Usage:
sub usage ($)
{
    return << "END_USAGE";
${ \(shift @_) }
Usage: comments.pl -db database
       -db database      Specify Understand database (required for 
                         uperl, inherited from Understand)
END_USAGE
}
#
#  For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl.html
# 
#  14-Dec-2000 KN
# 03-Aug-2001 DLL - updated for Understand::Gui::db()

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
if ( $db->language() !~ "C|Java" ) {
    die "This script is designed for C and Java only\n";
}

my @funcs = $db->ents("function ~unresolved ~unknown, procedure, method");
foreach my $func (sort {lc($a->longname()) cmp lc($b->longname());} @funcs) {
    if ($func->library() ne "Standard") {
        my $comment = $func->comments("after");
        if ($comment) {
            print "-----\n";
            print $func->longname(),":\n";
            print $comment,"\n";
            print "-----\n\n";
        }
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

