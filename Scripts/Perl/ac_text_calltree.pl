#
# Synopsis: Writes calltree for all root functions to text files.  
#
# Categories: Project Report, Graphics
#
# Languages: Ada, C/C++
#
# Description:
#  Saves to text file the call tree for all root functions (or a specified 
#  function).
#  The calltree filename written out can be loaded into Understand by double
#  clicking on the desired filename (when the command is run from Understand).
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
# 


sub usage($) {
    print shift,"\n";
    print "Usage: text_calltree.pl\n";
    print "  -db database   Specify database only if run from uperl\n";
    print "  -name name     Optional, function to report on, or all root functions by default\n";
    print "  -prototypes    Optional, include function prototypes in the output, if specified\n";
    print "  -rptdir dir    Optional, existing directory to write output, or cwd by default\n";
    print "  -showonce      Optional, show a function name at most once, if specified\n";
    print "  -unresolved    Optional, include unresolved functions in the output, if specified\n";
    print "  -verbose       Optional, display progress to stdout, if specified\n";
    print "  -all           Optional, generate call trees for all functions, not just root\n";
}


use Understand;
use Getopt::Long;
use strict;

my ($argDb,$argName,$argRptdir,$argUnresolved,$argVerbose,$argHelp,$argPrototypes,$argShowOnce,$all);
GetOptions
    (
     "db=s"	    => \$argDb,
     "h|help"	    => \$argHelp,
     "n|name=s"	    => \$argName,
     "p|prototypes" => \$argPrototypes,
     "r|rptdir=s"   => \$argRptdir,
     "showonce"     => \$argShowOnce,
     "unresolved"   => \$argUnresolved,
     "v|verbose"    => \$argVerbose,
     "a|all"        => \$all,
     ) or die usage("");
die usage("") if ($argHelp);


# open the database
my $db=Understand::Gui::db();
if (!$db) {
    die usage("Error, database not specified") unless $argDb;
    my $status;
    ($db,$status)=Understand::open($argDb);
    die print "Error opening database: $status\n" if $status;
}


# check language
my $language = $db->language();
die print "$language is currently unsupported" if ($language !~ /ada|c/i);


my @ents = ();
if ($argName) {
    @ents = lookupEnt($argName);
} else {
    @ents = getRootFuncs();
}

my $func_kinds;
if ($language =~ /Ada/i) {
   if ($argUnresolved) {
       $func_kinds = "function, procedure, entry";
   } else {
       $func_kinds = "function ~unresolved ~unknown, procedure ~unresolved ~unknown, entry ~resolved ~unknown";
   }
} else {
   if ($argUnresolved) {
       $func_kinds = "function, procedure";
   } else {
       $func_kinds = "function ~unresolved ~unknown, procedure ~unresolved ~unknown";
   }
}
@ents = $db->ents($func_kinds) if $all;

my %call_tree = ();
my %shown = ();
my $lastname = "";
my $namecount=0;
my $work_in_progress = 0;

foreach my $func (sort {$a->longname() cmp $b->longname();} @ents) 
{
    my ($outfile);
    my $longname;
    if ($func->longname() ne $lastname) {
      $lastname=$func->longname();
      $namecount=0;  # reset
    } else {
      $namecount++;
    }
    $work_in_progress = 0;
    %call_tree = ();    # reset for each root
    %shown = {};	# reset for each root
    # handle destructor as special case
    $longname = $func->longname();
    $longname =~ s/:/_/g;
    $longname =~ s/~/destructor_/;
    $longname =~ s/[^a-zA-Z0-9_~:\.]/_/g;
    $longname = $longname."_".$namecount if ($namecount);
    $longname = $longname."_calltree.txt";
    if ($argRptdir) {
      $outfile = $argRptdir."/".$longname;
    } else {
      $outfile = $longname;
    }
    if (not (open (RPT, "> $outfile"))) {
      print STDOUT "Can't open output file $outfile: $!\n"; 
      next;     # skip over this function
    }
    print STDOUT $func->longname()."  ";
    putFunc($func,0);
    print " saved to File: ".$outfile."\n";
    close (RPT);
}


# Return list of root functions.
# A root function is a function which has no callbys and at least one call.
sub getRootFuncs
{ 
  my @rootlist=();
  if ($argVerbose) { 
    print STDOUT "looking for root functions...\n";
  }    
  my @allfuncs;
  if ($language =~ /Ada/i) {
      @allfuncs = $db->ents("function ~unresolved ~unknown, procedure ~unresolved ~unknown, task ~unresolved ~unknown");
  } else {
      @allfuncs = $db->ents("function ~unresolved ~unknown");
  }
  foreach my $func (@allfuncs) {
    my @callbyrefs = $func->refs("callby ~inactive, useby ptr ~inactive");
    if (@callbyrefs == 0) {
        my @callrefs = $func->refs("call ~inactive, use ptr ~inactive");
        if (@callrefs > 0) {
          push @rootlist, $func;   
        }
    }
  }
  if ($argVerbose) { 
    print STDOUT scalar(@rootlist)." root functions found.\n"; 
  }      
  return @rootlist;
}


# Return list of functions matching the specified name.
sub lookupEnt 
{ 
    my($entname) = @_;
    my @entlist=();
   
    return () unless $entname;
    
    # check for word boundaries
    $entname = "\\b".$entname if ($entname !~ /^\*/);
    $entname = $entname."\\b" if ($entname !~ /\*$/);
  
    # replace wildcards * and ? with regex wildcards .* and ., respectively
    $entname =~ s/\*/\.\*/g; 
    $entname=~ s/\?/\./g; 

    my @allents = $db->ents("function ~unresolved ~unknown");
    foreach my $ent (@allents) {
      push @entlist, $ent if (($ent->name() =~ /$entname/) || ($ent->longname() =~ /$entname/));
    }
    return @entlist;
}


sub get_entity_line($) {
    my $ent = shift @_;
    return $ent->longname() unless ($argPrototypes);

    if ($language =~ /c/i) {
      return ($ent->type(), " ", $ent->longname(), " (",
        join(",", map {$_->type()." ".$_->name()} 
        $ent->ents("", "parameter")), 
		    ")");
    } elsif ($language =~ /ada/i) {
      return ($ent->longname(), " (",
        join(",", map {$_->name().": ",$_->type()}
		    $ent->ents("", "parameter")),
        ") return ", $ent->type());
    } elsif ($language =~ /fortran/i) {
      return ($ent->longname(), " (",
        join(",", map {$_->name()} $ent->ents("", "parameter")),
        ")");
    } elsif ($language =~ /java/i) {
      return ($ent->type(), " ", $ent->longname(), " (",
        join(",", map {$_->type()." ".$_->name()} 
		    $ent->ents("", "parameter")), 
        ")");
    }
    return $ent->longname();
}


sub putFunc {
  my ($func,$level) = @_;
  my (@calls,$call);
  return unless defined($func);

  print RPT "| " x $level;  
  print RPT get_entity_line($func);

  if ($call_tree{$func->id()}) {
    print RPT "  [recursive]\n";
    return;
  }
    
  @calls = $func->refs("call ~inactive, use ptr ~inactive",$func_kinds,1);
  if (scalar(@calls) && $argShowOnce && exists $shown{$func->id()}) {
    print RPT "  [duplicate]\n";
    return;
  }
  print RPT "\n";
    
  $shown{$func->id()} = 1 if ($argShowOnce);

  foreach my $call (@calls) {
    $call_tree{$func->id()}++;
    putFunc($call->ent(),$level+1);
    $call_tree{$func->id()}--;
  }

  if ($argVerbose) {
    $work_in_progress++;
    if (($work_in_progress % 100) == 0)   { print STDOUT $work_in_progress.","; }
    if (($work_in_progress % 1000) == 0)  { print STDOUT "\n"; }
  }
}