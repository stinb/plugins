#
# Sample Understand perl API program 
#
# Synopsis:   Reports all Fortran io units and any references to them.
#
# Categories: Project Report
#
# Languages: Fortran
#
# Usage:
sub usage ($)
{
    return << "END_USAGE";
${ \( shift @_ ) }
Usage: f_iounits -db database [-body] [-out file]
        -db database   Specify Understand database (required for 
                       uperl, inherited from Understand)
	-out file      Specify output file instead of stdout (optional)
END_USAGE
}

# Description:
#   Reports all variables and integers used as the 'unit' in an io statements (open,
#      close, read, write, backspace, rewind, endfile, inquire, unlock, wait).
#   Note: long type strings are truncated to $max_type_len (256)
#   Requires an existing Understand for Fortran database
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
my $bodyOpt;
my $help;
GetOptions(
	   "db=s"  => \$dbPath,
      "out=s" =>\$outFile,
	   "help" => \$help,
);

# help message
die usage("") if ($help);

# open the database 
my $db=openDatabase($dbPath);

# check language
if ( $db->language() !~ "Fortran" ) {
    die "This script is designed for Fortran only\n";
}

# use optional output file
if ($outFile) {
    open(RPT, "> $outFile") or die "Can't open output file $outFile: $!\n";
    print "Writing output to $outFile\n";
    select(RPT);
}

my $max_type_len = 256;

# report all io unit variables and their references
my @iounits;
@iounits = $db->ents("Variable");

foreach my $obj (sort {lc($a->longname()) cmp lc($b->longname());} @iounits) 
{
    # skip those that don't have any io useby relations
    next if (!$obj->refs("useby io"));

    print ( "\n".$obj->longname());
    # report objects type, if any (truncate if excessive length)
    if ($obj->type()) {
       if (length($obj->type()) > $max_type_len) { 
	       printf("  (%.*s...)", $max_type_len, $obj->type());
	    }   
	    else {  
	       print("  (".$obj->type().")");
	    }
    }
    print ("\n");
    
    # print each usage of the var
    foreach my $ref ($obj->refs())
    {
	     print ("    ".$ref->kindname().": ".$ref->ent()->name()
	       ." \t[File: ".$ref->file()->longname()
	       ." Line: " .$ref->line()."]\n");
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


# get declarein reference of specified entity
# there may be several, so get "best"
sub getDeclareinRef 
{
   my ($ent) =@_;
   return () unless defined($ent);
   
   my @decl=();
   my @declOrder = ("definein",
                    "declarein");
           
   foreach my $type (@declOrder)
   {
      @decl = $ent->refs($type);
      if (@decl) { last;  }
   }
   return @decl;
}
