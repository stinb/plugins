#
# Sample Understand perl API program
#
# Synopsis: Outputs a text callby tree for selected functions.
#
# Categories: Graphic, Project Report, Entity Report
#
# Languages: Ada, Fortran, C
#
# Usage:
sub usage($)
{
    return shift(@_) . <<"END_USAGE";
Usage: $0 -db database [-funcs file] [-output outdir] [-maxlevel level] [-unresolved] [-verbose]
  -db database	  Specify Understand database (required for uperl,
		  inherited if running Understand)
  -funcs function	  (optional) specify a list of function to report on.
  -maxlevel	  (optional) only print the first level depths of calls.
  -unresolved     if -unresolved is specified, unresolved functions 
		  will be included in the call tree.  By default,
		  unresolved functions are not included.
  -verbose	  if -verbose is specified, the roots of each tree will
		  be printed to stderr.
END_USAGE
    ;
}

# Description:
#  Saves a text callbt tree of the specified functions (by default, all root functions) to files, one callby tree per file.
#
#
#  For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl/
#  Refer to the documenation for details on using the perl API 
#  with Understand and for details on all Understand perl API calls.
# 
#  11-Jan-2001 TLB
#  05-Aug-2001 DLL - updated for Understand::Gui::db()
#
use Understand;
use Getopt::Long;
use strict;

my ($dbPath, $funcs, $depth, $unresolved, $verbose, $help);
GetOptions("db=s"	=> \$dbPath,
	   "f|funcs=s"	=> \$funcs,
	   "m|maxlevel=s" => \$depth,
	   "unresolved"	=> \$unresolved,
	   "v|verbose"	=> \$verbose,
	   "h|help"	=> \$help,
	   );

die usage("") if ($help);

my $db = openDatabase($dbPath);

my @ents;
my $qual = ($unresolved)? "" : "~unresolved ~unknown";
my $func_kinds = "function $qual, procedure $qual, subroutine $qual";
if ($funcs) {
    @ents = split(/,/,$funcs);
    @ents = map {get_func_ents($_)} @ents;
} else {
    @ents = find_root_funcs();
}

my (%call_list, $prev, $num);
foreach my $func (@ents) {

    my $file = flat_name($func->longname());
    $num = ($file eq $prev)? $num + 1 : "";
    $prev = $file;
    print STDERR $func->longname(), "\n" if ($verbose);
    print_calls_rec($func, 0, $depth);    
}

closeDatabase($db);

	
sub get_func_ents($) {
    my ($funcname) = @_;
    chomp $funcname;
    print 'looking for "', $funcname, "\"\n";
    return $db->lookup($funcname, $qual);
}

sub find_root_funcs()
{
    my @list;
    my @funcs = $db->ents($func_kinds);
    foreach my $func (@funcs) {
	my @callbys = $func->refs("call ~inactive, use ptr ~inactive");
	if (@callbys == 0) {
	    my @calls = $func->refs("callby ~inactive, useby ptr ~inactive");
	    if (@calls > 0) {
		push @list, $func;
	    }
	}				     
    }
    return @list;
}

sub flat_name($)
{
    $_ = shift @_;		# $_ is used for s/// beow.
    if ($db->language() =~ "C") {
	s/:/_/g;
	s/~/destructor_/;
    } elsif ($db->language() =~ "Ada") {
	s/./_/;
    } elsif ($db->language() =~ "Fortran") {
	
    } else {
	die usage("Unknown language");
    }
    return $_;
}

sub print_calls_rec($$;$)
{
    my ($func, $level, $maxlevel) = @_;
    
    return unless defined $func;
    $call_list{$func->id()}++;
  CALL: {  
      print  "| " x $level, $func->longname();
      if ($call_list{$func->id()} > 1){
		print  "  [recursive]\n";
		last CALL;
	  }
      print  "\n";
      last CALL if (defined $maxlevel and $level >= $maxlevel);
      
      my @calls = $func->refs("callby ~inactive, useby ptr ~inactive", $func_kinds, 1);
      foreach my $call (@calls) {
	  print_calls_rec($call->ent(), $level + 1, $maxlevel);
      }
  }
    $call_list{$func->id()}--;
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


