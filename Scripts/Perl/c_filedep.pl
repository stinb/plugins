#
# Sample Understand perl API program 
#
# Synopsis: Reports file dependencies for specified file(s) to stdout
#
# Categories: Project Report
#
# Languages: C
#
# Usage:
sub usage ($)
{
    return << "END_USAGE";
${ \( shift @_ ) }
Usage: filedep  -db database  [-file pattern] [-needs|-neededby]
      -db database       Specify Understand database (required for
	                 uperl, inherited from Understand)
      -name pattern      (optional) Specify a filename to match (i.e. file*.h)
                         If not specified, all files will be reported.
      -needs|-neededby   (optional) Specify -needs to report any file which the
                         current file needs/depends on.  Specify -neededby to
                         report other files that need the current file. If neither
                         option is set, both are reported.
      -defines		 (optional) Display dependencies for definitions 
			 as well as declarations.
END_USAGE
}

# Description:
#  Reports file dependencies for specified file(s) to stdout.  Only next-level
#  dependencies are reported.
#  Requires an existing Understand database.
#
#  For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl/
#  Refer to the documenation for details on using the perl API 
#  with Understand and for details on all Understand perl API calls.
# 
#  19-Jan-2001 TLB
#  04-Aug-2001 DLL - updated for Understand::Gui::db()

use Understand;
use Getopt::Long;
use strict;

my $dbPath;
my $filenamepattern;
my $needs;
my $neededby;
my $help;
my $defines;
GetOptions(
	   "db=s"   => \$dbPath,
           "name|file=s" => \$filenamepattern,
           "needs" => \$needs,
           "neededby" => \$neededby,
	   "d|define|defines" => \$defines,
	   "help" => \$help,
           );

# help message
die usage("") if ($help);

# open the database 
my $db=openDatabase($dbPath);

$defines = ($defines)? "definein, declarein" : "definein";

my $types = "file, object ~local ~private, type ~private, macro, enumerator, function, variable";

# check language
if ( $db->language() !~ "C" ) {
    die "This script is designed for C only\n";
}

# if neither option set, do both
if (! ($needs || $neededby))
{  
   $needs = 1;
   $neededby = 1;
}

my %unique = ();
# massage command line wildcards to form regex pattern
my $filename = buildPattern( $filenamepattern || "*" );
 
# find any/all files that match name pattern
my @files =();
foreach my $file ($db->ents("file ~unknown ~unresolved"))
{
   if ( ($file->name() =~ /$filename/) )
   {   push @files, $file;  }
}

foreach my $file (sort {$a->longname() cmp $b->longname();} @files)
{
   print "\n".$file->longname()."\n";
   # find dependencies for each matching file 
   # dependencies are any entity that the file includes, 
   # or any uses or calls of objects, types, macros
   if ($needs)
   {
      print "  Needs: \n";
      my %unique = ();    # reset
      my @ents = $file->ents("include, use, use ptr, set, modify, call, typed, declare", $types);

      foreach my $ent (@ents) {
         if ($ent->kind()->check("file")) {
	     print("    ",$ent->longname(),"\n") unless 
		 ($unique {$ent->id()}++);
         }
         else {			# get file where entity defined
	     my @decl = $ent->refs($defines);
	     foreach (@decl) {
		 next if ($_->file()->id() ==  $file->id());
		 print("    ",$_->file()->longname(), "\n") unless 
		     ($unique {$_->file()->id()}++);
	     }
         }
      }
      if (! keys(%unique)) # empty hash - no depends found
      {
          print "    None found.\n";
      }
   }
   
   # find files neededby each matching file 
   # entity neededby is any entity that the file is includedby, 
   # or any usebys or callbys of objects, types, macros
   if ($neededby)
   {
       print "  Needed by: \n";
       my %unique = ();    # reset
       my @ents = $file->ents("includeby, useby, useby ptr, setby, modifyby, callby, typedby, define", $types);
       foreach my $ent (@ents) {
	   if ($ent->kind()->check("file")) {
	       print("    ",$ent->longname(),"\n") unless
		   ($unique {$ent->id()}++);
	   }
	   else   # get file where entity defined
	   {
	       my @decl = $ent->refs($defines);
	       foreach (@decl) { 
		   next if ($_->file()->id() ==  $file->id());
		   print("    ",$_->file()->longname(),"\n") unless 
		       ($unique{$_->file()->id()}++);
		   
	       }
	   }
       }
       if (! keys(%unique)) # empty hash - none found
       {
	   print "    None found.\n";
       }
   }
   
}

closeDatabase($db);

# subroutines


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
