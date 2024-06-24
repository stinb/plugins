#
# Sample Understand perl API program 
#
# Synopsis: Writes short info about entities of a specified kind.
#
# Categories: Entity Report
#
# Languages: C
#
# Usage:
sub usage($) 
{
    return  <<"END_USAGE";
${ \( shift @_ ) }
Usage: undinfo.pl -db database <-class|-file|-func> pattern
 -db database        Specify Understand database (required for
                     uperl, inherited from Understand)
 -class pattern      (optional) Specify a class name or pattern to report.
                     Use -class * to report all classes or -class *patt* to 
                     match all classes whose name contain the string 'patt'.
 -file pattern       (optional) Specify a file name or pattern to report.
                     Use -file * to report all files or -file *patt* to match
                     all files whose name contain the string 'patt'.
 -func pattern       (optional) Specify a function name or pattern to report.
                     Use -func * to report all functions or -func *patt* to match
                     all functions whose name contain the string 'patt'.
END_USAGE
}


# Description:
#  Reports short information about specified type of entity whose name matches
#  a specified pattern string.
#  Requires an existing Understand database
#
# Class report contains:
#   Class name, link to where class is defined (double click to visit)
#   Comments, if any 
#   List of all public and protected members
# File report contains:
#   Full path name of file (double click to visit)
#   Comments, if any 
#   List of all global variables and their type
#   List of all global functions including their return types and parameters
# Function report contains:
#   Function name, return type, and any parameters
#   link to where function is defined (double click to visit)
#   Comments, if any 
#
#  For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl/
#  Refer to the documenation for details on using the perl API 
#  with Understand and for details on all Understand perl API calls.
# 
#  03-Jan-2001 TLB
#  06-Aug-2001 DLL - updated for Understand::Gui::db()
#
use Understand;
use Getopt::Long;
use strict;

my $dbPath;
my %opts=();
my $help;
GetOptions("db=s" => \$dbPath,
           "class:s" => \$opts{class},
           "file:s" => \$opts{file},
           "func:s" => \$opts{function},
	   "help" => \$help,
           );

# help message
die usage("") if ($help);

# make sure required args are specified
die usage("No pattern specified.\n") unless ($opts{class} || $opts{file} || $opts{function});

# open the database 
my $db=openDatabase($dbPath);

# check language
# language-specfic
if ( $db->language() !~ "C" ) {
    die "This script is designed for C/C++ only\n";
}

# find any/all types/names that match
my @ents =();
foreach my $type (keys(%opts))
{
  # replace command line wildcards * and ? with regex wildcards .* and ., respectively
  if ($opts{$type}) 
  {
      $opts{$type} = buildPattern($opts{$type});
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
    # language-specfic
  SWITCH: 
  {
      if ($ent->kind()->check("class") )    { print_class_info($ent);   last SWITCH; }
      if ($ent->kind()->check("function") ) { print_func_info($ent);    last SWITCH; }
      if ($ent->kind()->check("file") )     { print_file_info($ent);    last SWITCH; }
      print_info ($ent);  # default print info for any other entity
  }
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

sub print_class_info
{
    my ($ent) = @_;
 
    print "\n".$ent->kindname().": ".$ent->name();
    my @decl = $ent->refs ("definein");
    if (@decl) { print "  [File: ".$decl[0]->file()->longname()." Line: ".$decl[0]->line()."]"; }
    print "\n";
    my $comments = $ent->comments();
    if ($comments) { print $comments."\n"; }
    # output any public or protected members
    foreach my $ref (sort {$a->ent()->name() cmp $b->ent()->name();} $ent->refs("Define", "member ~private",1))
    {
         print "  ";
         if ($ref->ent()->type()) { print $ref->ent()->type()." "; }
         print $ref->ent()->name()."  (".$ref->ent()->kindname().")\n";
     }

}

sub print_file_info
{
    my ($ent) = @_;
 
    print "\n".$ent->kindname().": ". $ent->name()."\n";
    my $comments = $ent->comments();
    if ($comments) { print $comments."\n"; }
    print "Fullname of File: ".$ent->longname()."\n";
    
    # output all global objects
    foreach my $ref ($ent->refs("Define", "Object Global"))
    {
        print "  ";
        if ($ref->ent()->type()) {  print $ref->ent()->type()." "; }
        print $ref->ent()->name()."\n";
    }
    print "\n";
    
    # output all global functions, including their return types, and parameters
    foreach my $ref ($ent->refs("Define", "Function ~static"))
    {
        print "  ";
        if ($ref->ent()->type()) {  print $ref->ent()->type()." "; }
        print $ref->ent()->name();
        # output all params
        my $first = 1;
        print " ( ";
        foreach my $param ($ref->ent()->refs("Define", "Parameter", 1) )
        {
           if ($first) { $first = 0; }
           else        { print ", "; }
        
           if ($param->ent()->type()) {  print $param->ent()->type()." "; }
           print $param->ent()->name();   
        }
        print " )\n"; 
    }
    print "\n";
}

sub print_func_info
{
    my ($ent) = @_;    
    
    print "\n".$ent->kindname() . ": ";
    if ($ent->type()) 
    {   printf ("%s ", $ent->type());  }
    print $ent->name()." ";
    
    # output all params
    my $first = 1;
    print " ( ";
    foreach my $ref ($ent->refs("Define", "Parameter", 1) )
    {
        if ($first) { $first = 0; }
        else        { print ", "; }
        
        if ($ref->ent()->type()) {  print $ref->ent()->type()." "; }
        print $ref->ent()->name();   
    }
    print " )\n"; 
    
    my @decl = $ent->refs ("definein");
    if (@decl) 
    { 
        print "  Defined in File: ".$decl[0]->file()->longname()." , Line: ".$decl[0]->line();
        my @endref = $ent->refs("end");
        if (@endref)
        {   print "-".$endref[0]->line();   }
        print "]\n"; }

    my $comments = $ent->comments();
    if ($comments) { print $comments."\n"; } 

    print "\n"
}

# generic print entity info
sub print_info
{
    my ($ent) = @_;
    
    print "\n".$ent->kindname() . ": " . $ent->name();
    if ($ent->type()) 
    {   printf (" ( %s ) \n", $ent->type());  }
    else
    {   print "\n";  }
    my $comments = $ent->comments();
    if ($comments) { print $comments."\n"; }
    my @decl = $ent->refs ("definein");
    if (@decl) 
    { 
       printf ("Declared in %s [File: %s Line: %d]\n",
            $decl[0]->ent()->name(),
            $decl[0]->file()->longname(),
            $decl[0]->line() );     
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


