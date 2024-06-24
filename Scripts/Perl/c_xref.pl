# xref.pl
#
# Sample Understand perl API program 
#
# Synopsis: Report cross references for (known) entities matching the specified pattern
#
# Categories: Entity Report
#
# Languages: C
#
# Usage:
sub usage($) 
{
    my ($msg) = @_;

    return <<"END_USAGE";
${ \(shift @_ ) }
     perl xref.pl -db database -class pat|-file pat|-func pat|-var pat
 -db database     (required) Specify an Understand for C/C++ database.

 and choose one or more of the following:
 -class pat       (optional) Specify a class name or pattern to report.
                  Use -class * to report all classes or -class *pat* to  
                  match all classes whose name contain the string 'pat'. 
 -file pat        (optional) Specify a file name or pattern to report.
                  Use -file * to report all files or -file *pat* to  
                  match all files whose name contain the string 'pat'. 
 -func pat        (optional) Specify a function name or pattern to report.
                  Use -func * to report all functions or -func *pat* to  
                  match all functions whose name contain the string 'pat'. 
 -var pat         (optional) Specify a variable name or pattern to report.
                  Use -var * to report all variables or -var *pat* to 
                  match all variables whose name contains the string 'pat'.
END_USAGE
}

# Description:
#  Reports cross reference information (who references this entity) for the 
#  specified type of entity whose name matches the specified pattern string.
#  Double click to visit references within Understand.
#
#  For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl/
#  Refer to the documentation for details on using the perl API 
#  with Understand and for details on all Understand perl API calls.
# 
# 12-Feb-2001 TLB
# 06-Aug-2001 DLL - updated for Understand::Gui::db()
#
use Understand;
use Getopt::Long;
use strict;

my $dbPath;
my %opts=();
my $help;
GetOptions(
	   "db=s" => \$dbPath,
           "class:s" => \$opts{class},
           "file:s" => \$opts{file},
           "func:s" => \$opts{function},
           "var:s" => \$opts{object},
	   "help" => \$help,
           );

# help message
die usage("") if ($help);

die usage("No patterns specified.\n") unless ($opts{class} || $opts{file} || $opts{function} || $opts{object});

# open the database 
my $db=openDatabase($dbPath);


# find any/all types/names that match
my @ents =();
foreach my $type (keys(%opts))
{
  # build regex pattern from specified name
  if ($opts{$type}) 
  {
      $opts{$type} = buildPattern ($opts{$type});
  }
  foreach my $ent ($db->ents($type))
  {
    if ( $opts{$type} && ($ent->name() =~ /$opts{$type}/) )
    {   if ($ent->kind()->check("~unknown ~unresolved")) { push @ents, $ent;  } }
  }
}

#sort by kind and output data based on kind
foreach my $ent (sort {$a->kindname() cmp $b->kindname();} @ents)
{
    print_info ($ent);  # default print info for any other entity
}

closeDatabase($db);


# subroutines
 

# translate user specified pattern into regex pattern
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

sub print_info
{
    my ($ent) = @_;    
    
    print "\n".$ent->kindname() . ": ";
    print $ent->name()."\n";
      
    # may be language-specific
    my @xrefs = $ent->refs ("derive, callby ~inactive, declarein, definein, friendby, includeby, modifyby, setby, typedby, useby ~inactive");
    foreach my $xref (@xrefs)
    {
       print "    ".$xref->kindname().": ".$xref->ent()->name()." [File: ".$xref->file()->longname()." , Line: ".$xref->line();
       if ($xref->kind()->check("definein"))
       {
          my @endref = $ent->refs("end");
          if (@endref)
          {   print "-".$endref[0]->line();   }
       }
       print "]\n"; 
    }

    print "\n";
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

