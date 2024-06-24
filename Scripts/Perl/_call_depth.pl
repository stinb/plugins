#
# Sample Understand perl API program 
#
# Synopsis: Prints the call depth for each function.
#
# Categories: Project Report, Project Metrics  
# 
# Languages: All
# Usage:
sub usage($) 
{
    return @_[0] . <<"END_USAGE";
Usage: text_calltree.pl -db database
 -db database     Specify Understand database (required for 
                  uperl, inherited from Understand)
END_USAGE
}


# Description:
#  Prints out a list of functions with the maximum call depth for each one.
#
#  For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl/
#  Refer to the documenation for details on using the perl API 
#  with Understand and for details on all Understand perl API calls.
# 
#  11-Jan-2001 TLB
#  05-Aug-2001 DLL - updated for Understand::Gui::db()
#
use Understand;
use Getopt::Long;
use strict;

my $dbPath;
my $help;
GetOptions("db=s"   => \$dbPath,
	   "help" => \$help,
           );

# help message
die usage("") if ($help);

# open the database 
my $db=openDatabase($dbPath);

my %known = ();
my $kinds = "Function, Procedure, Subroutine";
my @ents = $db->ents($kinds);

foreach my $func (sort {$a->longname() cmp $b->longname();} @ents) 
{
    print $func->longname(), "\t", getDepth($func), "\n";
}

closeDatabase($db);


# subroutines

sub getDepth ($) {
    my ($func) = @_;

    return unless defined ($func);
    
    return "recursive" if ($known{$func->id()} == -1);
    return $known{$func->id()} if ( defined $known{$func->id()});
    
    $known{$func->id()} = -1;	# marker for recursion.
    
    my @calls = $func->refs("call ~inactive", $kinds, 1);
    foreach my $call (@calls) {
	my $depth = getDepth ($call->ent());
	return "recursive" if ($depth eq "recursive");
	$known{$func->id()} = $depth if ($depth > $known{$func->id()});
    }
    return ($known{$func->id()} += 1);
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

