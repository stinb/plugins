# Sample Understand perl API program 
#
# Synopsis: Report unused files 
#
# Categories: Project Report, Coding Standards
#
# Languages: All
#
# Usage:
sub usage($) 
{
    return  <<"END_USAGE";
${ \( shift @_ ) }
Usage: unused_files.pl -db database
-db database     Specify Understand database (required for
                 uperl, inherited from Understand)
END_USAGE
}

#  For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl/
#  Refer to the documentation for details on using the perl API 
#  with Understand and for details on all Understand perl API calls.
#
# 19-Sep-2001 DLL 

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

# hash to record usage
my %filesUsed = ();

# loop through files
my @files = $db->ents("File ~undefined ~unresolved");
foreach my $file (sort {$a->longname() cmp $b->longname();} @files) {    

    print $file->longname() . "\n";
    
    my $id = $file->id();
    $filesUsed{$id}->{ent} = $file;
    $filesUsed{$id}->{externalRefs} = 0;

    # loop through all refs in this file
    my @refs = $file->refs();
    foreach my $ref (sort {$a->ent()->longname() cmp $b->ent()->longname();} @refs) {    

	my $ent = $ref->ent();

	# find all definein, declarein, Call, Use Callby refs for this ent.
	my @useRefs = $ent->refs("setby,declarein,definein,call,use,useby,call,callby,includeby,withby,renameby");
	foreach my $useRef (sort {$a->ent()->longname() cmp $b->ent()->longname();} @useRefs) {    
	    
	    my $useFile = $useRef->file()->longname();
	    if ( $useFile ne $file->longname() ) {
		++($filesUsed{$id}->{externalRefs});
	    }
	}
    }
}

# print unused files
my @keys = keys (%filesUsed);
my @sortedKeys = sort {$filesUsed{$a}->{ent}->name() cmp $filesUsed{$b}->{ent}->name();} @keys;
foreach my $key (@sortedKeys) {
    my $ent = $filesUsed{$key}->{ent}; 
    my $externalRefs = $filesUsed{$key}->{externalRefs}; 

    #print quantity of external references
    #print "file: " . $ent->name() . 
	#" externalRefs:" . $externalRefs . "\n";

    # print any unused files
    if ( ! $externalRefs  && ( $ent->library() ne "Standard" )) { 
	print "unused file: " . $ent->name() . "\n";
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


