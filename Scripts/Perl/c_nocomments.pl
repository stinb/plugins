
# Sample Understand PERL API Script
#
# Synopsis: Lists functions with no comments
#
#   Lists functions with no comments.
#   uses longest comment (which means any comment), see documentation
#   for $func->comment() for other comment extraction options.
#
#   Requires an existing Understand for C++ database
#
# Categories: Project Report, Coding Standards
#
# Languages: C
#
# Usage:
sub usage($)
{
    return << "END_USAGE";
${ \( shift @_ ) }
Usage: nocomments.pl -db database
 -db database      Specify Understand database (required for
	           uperl, inherited from Understand)
END_USAGE
}

#  For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl.html
# 
#  14-Dec-2000 KN
#  05-Aug-2001 DLL - updated for Understand::Gui::db()


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

my @funcs = $db->ents("function ~unresolved ~unknown");
foreach my $func (sort {$a->longname() cmp $b->longname();} @funcs)
{
    my $comment = $func->comments("longest");
    if (! $comment)
    {
    foreach my $def ($func->refs("DefineIn"))
    {
           print $func->longname() . "\n\tFile: " . $def->file()->longname() . " Line: " . $def->line() . "\n";
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

