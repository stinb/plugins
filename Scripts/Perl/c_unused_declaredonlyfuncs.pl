# Sample Understand perl API program 
#
# Synopsis: Report unused functions that are declared only and optionally excluding those
#			of a specified name pattern
#
# Categories: Project Report, Coding Standards
#
# Languages: C
#
# Usage:
sub usage($) 
{
    return <<"END_USAGE";
${ \( shift @_ ) }
Usage: unused_funcs.pl -db database [-exclude pattern]
-db database     Specify Understand database (required for
                 uperl, inherited from Understand)
-exclude pattern (optional) Specify to exclude functions with the
                 specified naming pattern.  * and ? may be used
                 in the name pattern.
END_USAGE
}


#  For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl/
#  Refer to the documentation for details on using the perl API 
#  with Understand and for details on all Understand perl API calls.
#
# 08-Feb-2001 TLB
# 06-Aug-2001 DLL - updated for Understand::Gui::db()

use Understand;
use Getopt::Long;
use strict;

my $dbPath;
my $excludepattern;
my $help;
GetOptions(
	   "db=s" => \$dbPath,
           "exclude=s" => \$excludepattern,
	   "help" => \$help,
           );

# help message
die usage("") if ($help);

# open the database 
my $db=openDatabase($dbPath);

# check language
# language-specfic
if ( $db->language() !~ "C" ) {
    die "This script is designed for C/C++ only\n";
}
 
if ($excludepattern)  { $excludepattern = buildPattern($excludepattern); }

foreach my $func (sort {$a->longname() cmp $b->longname();} $db->ents ("function unresolved"))  
{    
   # an unused function: has no calls and no callbys
   if ( ! $func->refs("Call, Use Ptr, Callby, Useby Ptr, Inactive") ) 
   {
      # skip unless it matches the specified pattern (if applicable)
      if ($excludepattern)
      {  next unless ($func->longname() !~ /$excludepattern/);  }
      # get the declaration info
      my @decl = $func->refs ("Declarein");
      print ("  ".$func->longname()); 
      if (@decl)
      {   printf ("\t(File: %s Line: %d)",
               $decl[0]->file()->longname(),
               $decl[0]->line() );
      }
      printf (" (%d SLOC)",
	      $func->metric("CountStmtDecl") + $func->metric("CountStmtExe"));
      print ("\n");
   }
}

closeDatabase($db);


# subroutines 


# translate user-specified pattern into regex pattern
sub buildPattern 
{
   my ($pattern) = @_;
   
   return "" unless $pattern;
   # check for word boundaries
   if ($pattern !~ /^\*/ )  
      {$pattern = "\\b".$pattern; }
   if ($pattern !~ /\*$/ ) 
      {$pattern = $pattern."\\b"; }
  
   # replace wildcards * and ? with regex wildcards .* and ., respectively
   $pattern =~ s/\*/\.\*/g; 
   $pattern=~ s/\?/\./g; 
  
   return $pattern;
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


