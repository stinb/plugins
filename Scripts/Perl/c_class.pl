#
# Sample Understand perl API program 
#
# Synopsis:   Reports detailed information about a specified class or classes
#
# Categories: Entity Report
#
# Languages: C
#
# Usage:
sub usage ($)
{
    return << "END_USAGE";
${ \(shift @_) }
Usage: class.pl -db database [-class pattern] 
      -db database   Specify Understand database (required for 
                     uperl, inherited from Understand)
      -class pattern (optional) Specify a class name or pattern to report.
                     Use -class *patt* to match all classes whose name contains
                     the string "patt".  Omit option to report all classes. 
END_USAGE
}
# Description:
#  Reports to stdout detailed information about specified class(es) whose name
#  matches the specified pattern string or all classes if no pattern is given.
#  Requires an existing Understand for C/C++ database
#
# The class report contains:
#   Class name and link to where class is defined (double click to visit)
#   Associated comments, if any
#   List of all public, protected, and private members
#   List of all derived and base classes and where defined
#   List of all friends and where defined
#   List of all usage references
#
#  For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl/
#  Refer to the documenation for details on using the perl API 
#  with Understand and for details on all Understand perl API calls.
# 
#  22-Jan-2001 TLB
#  04-Aug-2001 DLL - updated for Understand::Gui::db()

use Understand;
use Getopt::Long;
use strict;

my $dbPath;
my $classnamepattern;
my $help;
GetOptions(
	   "db=s"   => \$dbPath,
           "class=s" => \$classnamepattern,
	   "help" => \$help,
          );

# help message
die usage("") if ($help);

# open the database 
my $db=openDatabase($dbPath);

# check language
if ( $db->language() !~ "C" ) {
    die "This script is designed for C only\n";
}


my $classname = $classnamepattern || "*";       # look for all classes if none specified

# massage pattern matching to check for word boundaries
if ($classname !~ /^\*/ )  
   {$classname = "\\b".$classname; }
if ($classname !~ /\*$/ ) 
   {$classname = $classname."\\b"; }
  
# replace wildcards * and ? with regex wildcards .* and ., respectively
$classname =~ s/\*/\.\*/g; 
$classname=~ s/\?/\./g;   

# find any/all classes that match name pattern
my @ents =();
foreach my $ent ($db->ents("Class ~unknown ~unresolved"))
{
   if ( ($ent->name() =~ /$classname/) )
   {   push @ents, $ent;  }
}


# alpha sort matching classes and output 
foreach my $ent (sort {$a->name() cmp $b->name();} @ents)
{
    print "\n".$ent->kindname().": ". $ent->name();
    my @decl = $ent->refs ("Definein");
    if (@decl)
    {
       print "\t [File: ".$decl[0]->file()->longname()." Line: ".$decl[0]->line()."]";
    }
    print "\n";
    # show comments, if any
    my $comments = $ent->comments();
    if ($comments) { print $comments."\n"; }
    # output any members
    # sort by kind using a hash of arrays
    my %members=();
    foreach my $ref  ($ent->refs("Define, Declare", "", 1))
    {
       push @{$members{$ref->ent()->kindname()} }, $ref->ent();
    }
    
    foreach my $membertype ( sort {$b cmp $a;} keys (%members))  # reverse sort by member type
    {
       if (@{$members{$membertype}})
       {
          print "\n    ".$membertype." : \n";
          foreach my $member (sort {$a->name() cmp  $b->name(); } @{$members{$membertype}})
          {
             print "        ";
             if ($member->type()) { print $member->type()." "; }
             print $member->name();
             print "\n";
          }
       }
    }
       
       
    # show base and derive classes (1-level only)
    my @bases = $ent->refs("Base");
    my @derived = $ent->refs("Derive");
    if (@bases)  {  print "\n    Base Classes:\n";  }
    foreach my $base  (sort {$a->ent()->name() cmp  $b->ent()->name(); } @bases)
    {
        print "        ".$base->ent()->name();
        my @decl = $base->ent()->refs ("Definein");
        if (@decl)
        {
            print "\t [File: ".$decl[0]->file()->longname()." Line: ".$decl[0]->line()."]";
        }
        print "\n";
    }
    if (@derived) { print "\n    Derived Classes:\n"; }
    foreach my $derive  (sort {$a->ent()->name() cmp  $b->ent()->name(); } @derived)
    {
        print "        ".$derive->ent()->name();
        @decl = $derive->ent()->refs ("Definein");
        if (@decl)
        {
            print "\t [File: ".$decl[0]->file()->longname()." Line: ".$decl[0]->line()."]";
        }
        print "\n";
    }

    
    # show friends
    my @friends = $ent->refs("Friend", "", 1);         # unique friend refs only
    if (@friends)  {  print "\n    Friends:\n";  }
    foreach my $friend  (sort {$a->ent()->name() cmp  $b->ent()->name(); } @friends)
    {
        print "        ".$friend->ent()->name();
        my @decl = $friend->ent()->refs ("Definein");
        if (@decl)
        {
            print "\t [File: ".$decl[0]->file()->longname()." Line: ".$decl[0]->line()."]";
        }
        print "\n";
    }
    
    # show any other usage references 
    my @refs = $ent->refs("Useby, Setby, Typedby");
    if (@refs)  {  print "\n    Usage References:\n";  }
    foreach my $ref  (@refs)
    {
        print "        ".$ref->kindname().": ".$ref->ent()->name()." \t[File: ".$ref->file()->longname()." Line: ".$ref->line()."]\n";
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

