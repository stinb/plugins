#
# Sample Understand perl API program 
#
# Synopsis: Writes calltree for all root functions to text files.  
#
# Categories: Project Metrics, Entity Metrics, Graphics
#
# Languages: All
# Usage:
sub usage($) 
{
    return shift(@_) . <<"END_USAGE";
Usage: $0 -db database [-name functionnamepattern ] [-rptdir dirpath] [-unresolved]
 -db database     Specify Understand database (required for 
                  uperl, inherited from Understand)
 -name funcnamepattern   (optional) specify a function name (optional). If not
                  specified, reports for all root functions will be generated.
                  A 'root' function  is one which has calls but is not called 
                  by any other function.
 -metric metric   (optional) prints the specified metric after each function 
		  call.  If not specified, no metric is printed.
 -rptdir dirpath  (optional) specify a path to an existing directory where 
                  reports will be written.  If not specified, reports are 
                  generated to the current working directory.
 -unresolved      (optional) if -unresolved is specified, unresolved functions 
                  will be included in the call tree.  By default, unresolved 
                  functions are not included.
 -verbose         specify to output progress reports to stdout during processing
END_USAGE
}


# Description:
#  Saves to text file the call tree and metric for all root functions (or a specified 
#  function).
#  The calltree filename written out can be loaded into Understand by double
#  clicking on the desired filename (when the command is run from Understand).
#  Requires an existing 1.4 B96 or later version of an Understand for C database
#
# Notes on file naming:
#  In general files are named functionname_calltree.txt
#  For C++ member functions, files are named classname__memberfunctionname_calltree.txt
#  For C++ destructor functions, the destructor symbol is replaced with the text
#  "destructor",so the call tree for destructor function classA::~funcA will be
#  saved to the file classA__destructor_funcA_calltree.txt
#  Overloaded operators (such as operator>>) are skipped.  Any functions skipped
#  will be reported to STDOUT.
#
#  For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl/
#  Refer to the documenation for details on using the perl API 
#  with Understand and for details on all Understand perl API calls.
# 
#  11-Jan-2001 TLB
#  05-Aug-2001 DLL - updated for Understand::Gui::db()
#  03-Jan-2002 Evan Knop - added metric-gathering info.
#
use Understand;
use Getopt::Long;
use strict;

my $dbPath;
my $funcname;
my $rptdir;
my $unresolved;
my $metric;
my $verbose;
my $help;
my $lang;
GetOptions("db=s"   =>   \$dbPath,
           "name=s" =>   \$funcname,
           "rptdir=s" => \$rptdir,
           "unresolved" =>\$unresolved,
	   "m|metric=s" =>\$metric,
           "verbose" =>  \$verbose,
	   "h|help" =>   \$help,
           );

# help message
die usage("") if ($help);

# open the database 
my $db=openDatabase($dbPath);

my @ents = ();
my $func_kinds = "function, procedure, subroutine";

if ($funcname) { @ents = lookupEnt ($funcname);}
else           { @ents = getRootFuncs();}

$func_kinds =~ s/(?=,|$)/ ~unresolved ~unknown/g if ($unresolved);

my $metric_op = ($metric =~ /Count/)?
    sub { return $_[0] + $_[1] } :
    sub { return "unsupported" };

my %call_tree = ();
my $lastname = "";
my $namecount=0;
my $work_in_progress = 0;
my %cache;

foreach my $func (sort {$a->longname() cmp $b->longname();} @ents) 
{
   my ($outfile);
   my $longname;
   
   if ($func->longname() ne $lastname)
   {
     $lastname=$func->longname();
     $namecount=0;  # reset
   }
   else
   {
      $namecount++;
   }
   my $level = 0;
   $work_in_progress = 0;
   %call_tree = ();    #reset for each root
   if ($func->longname() =~ /[^a-zA-Z0-9_~:]/)
   {
      print STDOUT "skipping function: ".$func->longname()."\n";
      next;
   }; 
   # handle destructor as special case
   $longname = $func->longname();
   
   print STDOUT "$longname :\n";
   $longname =~ s/:/_/g;
   $longname =~ s/~/destructor_/;
   if ($namecount)
   {
     $longname = $longname."_".$namecount;
   }
   $longname = $longname."_calltree.txt";
   if ($rptdir)
   {   $outfile = $rptdir."/".$longname;  }
   else
   {   $outfile = $longname;  }
   if (not (open (RPT, "> $outfile") ))
   { 
      print STDOUT "Can't open output file $outfile: $!\n"; 
      next;     # skip over this function
   }
   print STDOUT $func->longname()."  ";
   my ($calltree) = getMetrics($func);
   putFunc (0, @$calltree);
   print " saved to File: ".$outfile."\n";

   close (RPT);
}

closeDatabase($db);


# subroutines

# return list of root functions
# a root function is a function which has no callbys and at least one call
sub getRootFuncs
{ 
   my @rootlist=();

   if ($verbose) { print STDOUT "looking for root functions...\n"; }    
   my @allfuncs = $db->ents("function ~unresolved ~unknown");
   foreach my $func (@allfuncs)
   {
      my @callbyrefs = $func->refs("callby ~inactive, useby ptr ~inactive");
      if (@callbyrefs == 0)
      {
         my @callrefs = $func->refs("call ~inactive, use ptr ~inactive");
         if (@callrefs > 0)
         {  
            push @rootlist, $func;   
         }
      }
    }  
    if ($verbose) { print STDOUT scalar(@rootlist)." root functions found.\n"; }      
    return @rootlist;
}

# return list of functions matching the specified name
sub lookupEnt 
{ 
   my($entname) = @_;
   my @entlist=();
   
   return () unless $entname;
    
   # check for word boundaries
   if ($entname !~ /^\*/ )  
      {$entname = "\\b".$entname; }
   if ($entname !~ /\*$/ ) 
      {$entname = $entname."\\b"; }
  
   # replace wildcards * and ? with regex wildcards .* and ., respectively
   $entname =~ s/\*/\.\*/g; 
   $entname=~ s/\?/\./g; 

   my @allents = $db->ents($func_kinds);
   foreach my $ent (@allents)
   {
      if ( ($ent->name() =~ /$entname/) ||
           ($ent->longname() =~ /$entname/) )
      {  push @entlist, $ent;   }

    }  
    return @entlist;
}

sub putFunc ($@)
{
   my ($level, @tree) = @_;
   
   my $ent = $db->ent_from_id($tree[0]);
   print RPT  "| " x $level, $ent->longname();
   
   if ($tree[2] eq "recursive") {
       print RPT " [recursive]\n";
       return;
   }

   if ($metric) {
       print RPT ": ", $ent->metric($metric), " (", $tree[1], ")";
   }
   print RPT "\n";

   foreach my $call (@{$tree[2]}) {
       putFunc ($level + 1, @{$call});
   }
   return;
}

sub getMetrics ($)
{
    my ($func) = @_;
    my (@calls, @results, %funcs, $agregate, $out);
    
    return unless defined($func);
    my $metval = 0;
	$metval = $func->metric($metric) if $metric;
	
    $call_tree{$func->id()}++;
    $funcs{$func->id()} = $metval;
    
    # don't count recursive functions again...
    return ([$func->id(), $metval, "recursive"], %funcs)
	if ($call_tree{$func->id()} > 1);
    
    @calls = $func->refs("call ~inactive, use ptr ~inactive", $func_kinds, 1);
    foreach my $call (@calls) {
	my ($child, %offspring) = getMetrics($call->ent());
	my ($s, $v);
	push @results, $child;
	# remove from call tree hash.  Easier to do once here.
	$call_tree{$call->ent()->id()}--;
	while ( ($s, $v) = each %offspring ) {
	    $funcs{$s} = $v;
	}
    }
    
    if ($verbose && (++$work_in_progress % 200 == 0)) {
	print STDOUT $work_in_progress, ", ";
    }
    
    map { $agregate = &$metric_op ($agregate, $_) } values %funcs;
    
    return ([$func->id(), $agregate, \@results], %funcs);
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
    $db->close() if (!Understand::Gui::db());
}
