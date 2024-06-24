# Sample Understand PERL API Script
#
# Synopsis: Extract and report named comment lines
#
#   Examines all functions and their associated comments, looking 
#   for comments that match the given name string.  Reports 
#   all such strings contained in each function and the converse,
#   all functions that contain each comment string.
#
# Categories: Project Report, Coding Standards
#
# Languages: All
#
# Usage:
sub usage ($)
{
    return << "END_USAGE";
${ \( shift @_ ) }
Usage: namedcomments.pl -db database [-value value] [-name name]
 -db database  Specify Understand database (required for 
               uperl, inherited from Understand)
 -value value  Filter regex such as "DM.46"
 -name name    Alternate name regex 
END_USAGE
}

#  For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl.html
# 
# 27-Jul-2001 DLL
# 06-Aug-2001 DLL - updated for Understand::Gui::db()

use Understand;
use Getopt::Long;
use strict;

my $dbPath;
my $nameRegex;
my $valueRegex;
my $help;
GetOptions(
	   "db=s" => \$dbPath,
	   "name=s" => \$nameRegex,
	   "value=s" => \$valueRegex,
	   "help" => \$help,
          );

die usage("") if $help;

# open the database 
my $db=openDatabase($dbPath);


if ( !$nameRegex ) {
   #$nameRegex = '[ a-zA-Z0-9$_/-]*\s*: ';   #all
   $nameRegex = '^[ a-zA-Z0-9$_/-]*\s*: ';   #anchored, all 
   #$nameRegex = '^\s*Requirement\s*:' ;     #anchored, only "Requirement"
   #$nameRegex = '^\s*(Author|Requirement)\s*:' ;  #anchored, "Author" or "Requirement"
}
if ( !$valueRegex ) {
    $valueRegex = '.+';   #all
    #$valueRegex = 'Billy Bob';   #just containing "Billy Bob"
}

my $language = $db->language();
my %matches=();
my @ents = ();
my %namedComments= ();
my %entsWithComments= ();

print "nameRegex=$nameRegex\n";
print "valueRegex=$valueRegex\n";


if ( $db->language() =~ "^Ada" ) {
    @ents = $db->ents("function ~unresolved ~unknown,procedure,file,component,package,task");
}
if ( $db->language() =~ "^C" ) {
    @ents = $db->ents("function ~unresolved ~unknown");
}

foreach my $ent (sort {lc($a->longname()) cmp lc($b->longname());} @ents) {

    # skip Ada standard libaray ents
    next if ($ent->library() eq "Standard");
    # get comments, skip if none
    #my $comment=$ent->comments("after","default","declarein body");
    my $comment=$ent->comments("after","default");
    next if (!$comment);

    # break into lines and loop through lines in the comment
    my @str = split(/\n/,$comment);
    foreach my $s (@str) {
	
	# skip ! name match
	next if ($s !~ $nameRegex);

	# set name = that matched by $nameRegex
	my $name = $&;
	
	# set value to all that follows matched name
	my $value = substr($s,length($name));

	# skip if ! value match
	next if ($value !~ $valueRegex);
	
	addToHashes($ent,$name,$value);
    }
}
    
# print them out
report($db);

# the-end
closeDatabase($db);
    


# subroutines


sub report()
{
    my ($db) = @_;


    print "\norder by entity:\n\n";
    foreach my $key ( sort {lc($entsWithComments{$a}->{'ent'}->longname()) cmp lc($entsWithComments{$b}->{'ent'}->longname()); } keys (%entsWithComments) ) {
	print "  " . getDecoratedEntName($entsWithComments{$key}->{'ent'}) . "\n";
	printNamedCommentsByEntity($entsWithComments{$key}->{'ent'});
    }

    print "\norder by named comment:\n\n";
    foreach my $key ( sort {lc($namedComments{$a}->{'name'}) cmp lc($namedComments{$b}->{'name'}); } keys (%namedComments) ) {
	print "  " . $namedComments{$key}->{'name'} . "\n";
	printNamedCommentsByName($namedComments{$key}->{'name'});
    }


    return;


}





sub printNamedCommentsByName()
{
    my ($name) = @_;

    foreach my $key ( sort {lc($matches{$a}->{'name'}) . lc($matches{$a}->{'value'}) cmp lc($matches{$b}->{'name'}) . lc($matches{$b}->{'value'}); }
		      keys (%matches) ) {

	next if ( $matches{$key}->{'name'} ne $name );
	print "   " . getDecoratedEntName($matches{$key}->{'ent'}) . " " 
	    . $matches{$key}->{'value'} . "\n";
    }

}


sub printNamedCommentsByEntity()
{
    my ($ent) = @_;

    foreach my $key ( sort {lc($matches{$a}->{'name'}) . lc($matches{$a}->{'value'}) cmp lc($matches{$b}->{'name'}) . lc($matches{$b}->{'value'}); }
		      keys (%matches) ) {

	next if ( $matches{$key}->{'ent'} != $ent );
	
	    print "   " . $matches{$key}->{'name'} . " : " 
		. $matches{$key}->{'value'} . "\n";
    }

}

sub getDecoratedEntName($)
{
    my ($ent) = @_;


    my $name = $ent->longname();
    my ($file,$line,$column) = getDeclLocation($ent);
    my $kind = $ent->kindname();

    return( $kind." ".$name." ".$file."{".$line."}" );
}


sub getDeclLocation($)
{
    my ($ent) = @_;

    # get declarein reference 
    my ($ref)=$ent->refs("Declarein Body,Declarein Spec","",1);

    # return line/column/file where declared
    return( $ref->file()->name(), $ref->line(), $ref->column()) if $ref;
}

sub addToHashes()
{
    my ($ent,$name,$value) = @_;

    # remove trailing colon if any from name
    $name =~ s/:\s*$//;

    # trim leading and trailing spaces from strings
    $name =~ s/^\s+//; $name =~ s/\s+$//;
    $value =~ s/^\s+//; $value =~ s/\s+$//;

    #declare and bless this hash reference 
    my $hashref = {};
    bless ($hashref);

    # set hash components
    $hashref->{'id'} = $ent->id(); 
    $hashref->{'ent'} = $ent; 
    $hashref->{'name'} = $name;
    $hashref->{'value'} = $value;

    # add it to hash using unique key
    $matches{$ent->id() . $name . $value} = $hashref;

    # remembers all the unique named comments names
    $namedComments{$name} = $hashref;
    
    # remember all the unique entities with named comments
    $entsWithComments{$ent->id()} = $hashref;

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

