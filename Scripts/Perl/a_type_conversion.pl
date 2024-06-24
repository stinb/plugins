#
# Sample Understand perl API program 
#
# Synopsis: Reports all explicit Ada type conversions
#
# Categories: Project Report, Coding Standards
#
# Languages: Ada
#
# Usage:
sub usage ($)
{
    return << "END_USAGE";
${ \( shift @_ ) }
Usage: type_conversion -db database [-out file]
        -db database   Specify Understand database (required for 
                       uperl, inherited from Understand)
	 -out file      Specify output file instead of stdout (optional)
END_USAGE
}

# Description:
#   Reports (explicit) type conversions within a program.  At the 
#   time of writing (05 July 2002), U4Ada does not record implicit 
#   conversions.
#
#  For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl/
#  Refer to the documenation for details on using the perl API 
#  with Understand and for details on all Understand perl API calls.
# 
#  05-Jul-2002 Evan Knop
#

use Understand;
use Getopt::Long;
use strict;

my ($dbPath, $outfile, $help);
GetOptions(
	   "db=s"	=> \$dbPath,
	   "out=s"	=> \$outfile,
	   "help"	=> \$help,
       );

# help message
die usage("") if ($help);

# open the database
my $db = openDatabase($dbPath);

# check language
unless ( $db->language() =~ /ada/i ) {
    die "This script is designed for Ada only\n";
}

# use $outfile, if we have it.
if ($outfile) {
    open (RPT, "> $outfile") or die "Can't open output file $outfile: $!\n";
    print "Writing output to $outfile\n";
    select (RPT);
}

foreach my $ent ( $db->ents() ) {
    my @conversions = $ent->refs("Convert");
    if (scalar @conversions) {
    my ($type) = $ent->ents("typedby", "type");
    my $typename = ($type)? " (".$type->name().")" : "";
    print $ent->longname(), $typename, " has ", scalar @conversions, " conversions: \n";
        foreach my $ref (@conversions) {
	    my $refent = $ref->ent();
	    my ($type) = $refent->ents("typedby", "type");
	    my $typename = ($type)? " (".$type->name().")" : "";
	    print "  in ", $refent->longname(), $typename, " [", $ref->file()->longname(), " line ", $ref->line(), "]\n";
	}
    }
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
