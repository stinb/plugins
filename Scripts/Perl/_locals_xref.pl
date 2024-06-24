#
# Sample Understand perl API program 
#
# Synopsis: Reports all local variables and any references to them.
#
# Categories: Project Report
#
# Languages: All
#
# Usage:
sub usage ($)
{
    return << "END_USAGE";
${ \( shift @_ ) }
Usage: locals_xref.pl -db database [-out file]
        -db database   Specify Understand database (required for 
                       uperl, inherited from Understand)
	-out file      Specify output file instead of stdout (optional)
	-longnames     Show long names for entitys 
	-longkinds     Show long names for reference kinds
	-longfiles     Show long names for file names
END_USAGE
}
# Description:
#   Reports all local variables and any references to them.
#
# Ada:
#   Note: long type strings are truncated to $max_type_len (256)
#   Requires an existing Understand for Ada database
#   Standard library entities are ignored
#
#  For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl/
#  Refer to the documenation for details on using the perl API 
#  with Understand and for details on all Understand perl API calls.
# 


use Understand;
use Getopt::Long;
use strict;

    
# get command line args 
my $dbPath;
my $outFile;
my $longnames;
my $longkinds;
my $longfiles;
my $help;
GetOptions(
	   "db=s"  => \$dbPath,
           "out=s" => \$outFile,
	   "longnames" => \$longnames,
	   "longkinds" => \$longkinds,
	   "longfiles" => \$longfiles,
	   "help" => \$help,
	   );

# help message
die usage("") if ($help);

# open the database 
my $db=openDatabase($dbPath);

my $language = $db->language();

# use optional output file
if ($outFile) {
    open(RPT, "> $outFile") or die "Can't open output file $outFile: $!\n";
    print "Writing output to $outFile\n";
    select(RPT);
}


# report all local variables and their references

my $max_type_len = 256;
my @locals = ();

if ( $language =~ "Ada" ) {
    @locals = $db->ents("Object Local," .
			    "Task Local," .
			    "Constant Local," .
			    "Exception Local");
}
if ( $language =~ "C" ) {
    @locals = $db->ents("Object Local");
}


    
foreach my $obj (sort {lc($a->longname()) cmp lc($b->longname());} @locals) 
{

    # skip ada standard library objects
    next if ($obj->library() eq "Standard");
    

    # print object's name
    my $name = $longnames ? $obj->longname() : $obj->name();

    print "\n". $name ;

    # report objects type, if any 
    if ($obj->type()) {
	#truncate to max_type_length 
	my $type = substr($obj->type(),0,$max_type_len);
	print "  (" . $type . ")";
    }
    print ("\n");
    
    # print each usage of the local var
    foreach my $ref (sort {lc($a->kindname()) cmp lc($b->kindname());} $obj->refs()) 
    {
	# print it out
	#my $kindname=$ref->kind()->longname();
	#my $kindname=$ref->kindname();
	my $kindname = $longkinds ? $ref->kind()->longname() : $ref->kindname();
	my $filename = $longfiles ? $ref->file()->longname() : $ref->file()->name();
	my $entname = $ref->ent()->name();
	my $line = $ref->line();
	print "    $kindname: $entname \t$filename\{$line\}\n";
    }
}
    
close(RPT) if $outFile;
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
	my $status = undef;
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
