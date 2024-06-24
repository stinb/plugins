#
# Sample Understand perl API program 
#
# Synopsis: Reports Information Browser data for specified entities
#
# Categories: Entity Report
#
# Languages: Ada, Fortran, C
#
# Usage:
sub usage($)
{
    return << "END_USAGE";
${ \( shift @_ ) }
Usage: info.pl -db database  -name pattern -opts options
 -db database     Specify Understand database (required for
	          uperl, inherited from Understand)
 -name pattern    (required) Specify an entity name or pattern to report.
                  Use -name *patt* to match all entities whose name contains 
                  the string "patt".  
 -opts options    (optional)  Specify a quoted string containing parameters
                  used to create the text. The format of the options string 
                  is "name=value" or "{field-name}name=value". Multiple 
                  options are separated  with a semicolon.  Spaces are 
                  allowed and are significant between multi-word field names, 
                  whereas, case is not significant. An option that specifies 
                  a field name is specific to that named field of the Info 
                  Browser. The available field names are exactly as they  
                  appear in the Info Browser. When a field is nested within 
                  another field, the correct name is the two names combined.
                  For example, in C++, the field Macros within the field  
                  Local would be specified as "Local Macros".
 
                  The following option is currently available only without 
                  a field name.
                  Indent  - this specifies the number of indent spaces to 
                  output for each level of a line of text. The default is 2.

                  All other options require a field name.  See $ent->ib() in 
                  http://www.scitools.com/perl/Understand.html for a 
                  descriptive list of all field names.
END_USAGE
}

# Example Usage:
#   uperl info.pl -db d:\examples\sample.udcc -name extract* 
#   -opts "indent=4;{calls}levels=4;{calls}parameters=on;{defined in}fullnames=on" 
#
# Description:
#  Reports to stdout the Information Browser for the specified entities whose name
#  matches the specified pattern string.
#  Requires an existing Understand database
#
#  For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl/
#  Refer to the documenation for details on using the perl API 
#  with Understand and for details on all Understand perl API calls.
# 
# 15-Mar-2001 DP
# 15-Mar-2001 TLB 
# 05-Aug-2001 DLL - updated for Understand::Gui::db()

use Understand;
use Getopt::Long;
use strict;

my $dbPath;
my $entname;
my $options;
my $help;
GetOptions(
	   "db=s"   => \$dbPath,
           "name=s" => \$entname,
           "opts:s" => \$options,
	   "help" => \$help,
           );

# help message
die usage("") if ($help);

# open the database 
my $db=openDatabase($dbPath);

# make sure required args are specified
die usage("") unless ($entname);


# massage pattern matching to check for word boundaries
if ($entname !~ /^\*/ )  
   {$entname = "\\b".$entname; }
if ($entname !~ /\*$/ ) 
   {$entname = $entname."\\b"; }

# make any . in name a word boundary
$entname =~ s/\./\\b/g;  

# replace wildcards * and ? with regex wildcards .* and ., respectively
$entname =~ s/\*/\.\*/g; 
$entname=~ s/\?/\./g;   

# in Ada or FORTRAN, ignore case.
$entname = '(?i)' . $entname if ($db->language() =~ m/Ada|Fortran/i);

#use default options if none defined
$options = "{calls}levels=1;{callbys}levels=1" unless $options;

# find all entities that match the name pattern
my @ents = grep {$_->name() =~ /$entname/} $db->ents();

# output alphabetically
foreach my $ent (sort {$a->name() cmp $b->name();} @ents)
{
   print $ent->ib($options),"\n";
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

