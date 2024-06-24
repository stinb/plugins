#
# Sample Understand perl API program 
#
# Synopsis: View filtered list of entities and optionally include comments
#
# Categories: Project Report, Entity Report
#
# Languages: Ada, Fortran, C
# Usage:
sub usage ($)
{
    return << "END_USAGE";
${ \( shift @_ ) }
Usage: dump -db database  [-name namepattern] [-sort decl|file|kind|name] [-kind kindfilter] 
                        [-declaredin filter] [-declaredinfile filter] [-comments]
 -db database       (required) Specify an Understand database.
 -name namepattern  (optional) Specify an entity name or pattern to report.
                    If omitted, all entities will be reported.
 -sort              (optional) Specify the sort order in which to display the list.
                    Choose one of:
                       name   to sort by entity (short) name (default)
                       kind   to sort by kind of entity
                       file   to sort by filename where declared
                       decl   to sort by name of declaration entity
                    If not specified, entities are sorted by name.
 -kind  kindfilter  (optional)  Used for filtering results to only those of a 
                    specified entity kind.  Specify the kind keyword to filter by. 
                    For example: 
                         -kind "public member"  will show only public member entities.
                    See  http://www.scitools.com/perl/Understand.html#kind_filters  
                    for the list of kind filters for each language
 -declaredin filter (optional) Used to filter results to contain only those 
                    entities that are declared in the specified entity. 
                    * and ? wildcards are allowed in the filter name.
 -declaredinfile filter (optional) Use to filter results to contain only 
                    those entities that are declared in the specified file. 
                    * and ? wildcards are allowed in the filter name. 
 -comments          (optional)  Specify to also include any comments that
                    may be associated with the entity.  By default, no 
                    comments are reported.  (comment association available
                    in Understand for C/C++ and Ada only)
END_USAGE
}

# Description:
#  View list of all entities with various filtering and sorting options.  
#  If -name is specified, only those entities whose name matches the specified
#  pattern will be listed.  The -sort option can be used to sort to the list
#  either by name (default), entity kind, declaration entity name, or filename
#  where entity is declared.
#  For C/C++,  pattern matching is case sensitive; for Ada and Fortran, pattern
#  matching is case insensitive.
#  Requires an existing Understand database
#
#  For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl/
#  Refer to the documenation for details on using the perl API 
#  with Understand and for details on all Understand perl API calls.
# 
# 30-Jan-2001 TLB
#  03-Aug-2001 DLL - updated for Understand::Gui::db()
#
use Understand;
use Getopt::Long;
use strict;

my $dbPath;
my $namepattern;
my $sortorder;
my $kindfilter;
my $declfilter;
my $filefilter;
my $comments;
my $help;
GetOptions(
	   "db=s" => \$dbPath,
           "name=s" => \$namepattern,
           "sort=s" => \$sortorder,
           "kind=s" => \$kindfilter,
           "declaredin:s" => \$declfilter,
           "declaredinfile:s" => \$filefilter,
           "comments" => \$comments,
	   "help" => \$help,
           );

# help message
die usage("") if ($help);

# open the database 
my $db=openDatabase($dbPath);


$namepattern = "*" unless $namepattern;
$sortorder = "name" unless $sortorder;

$namepattern = buildPattern ($namepattern);
$declfilter = buildPattern ($declfilter);
$filefilter = buildPattern ($filefilter);

my $language = $db->language();
my %entHash = ();
my %declHash = ();

# min field widths for output - these could be changed or even made into options later on
my $min_name_width = 25;
my $min_kind_width = 25;
my $min_decl_width = 25;

# get matching entities
LANGUAGE_SWITCH: 
{
   if ($language =~ /ada/i)     { loadAdaEnts($namepattern);      last LANGUAGE_SWITCH; }
   if ($language =~ /fortran/i) { loadFortranEnts($namepattern);  last LANGUAGE_SWITCH; }
   # default is C
   loadCEnts($namepattern);  
}

# output report header
    printf "%-*s %-*s %-*s %s\n",
       $min_name_width,
       "ENTITY NAME",
       $min_kind_width,
       "ENTITY KIND",
       $min_decl_width,
       "DECLARED IN",
       "FILE/LINE WHERE DECLARED";

# report ordered results (based on user-specified sort order)
SORT_SWITCH: 
{
   if ($sortorder =~ /decl/i)    { reportDeclOrder();  last SORT_SWITCH; }
   if ($sortorder =~ /file/i)    { reportFileOrder();  last SORT_SWITCH; }
   if ($sortorder =~ /kind/i)    { reportKindOrder();  last SORT_SWITCH; }
   # default sort is name order
   reportNameOrder();  
}

closeDatabase($db);


# subroutines

# convert glob-like pattern to regex pattern
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

# return name of declaration file or null if none
# used for sorting by decl file
sub getDeclFileName
{
   my ($key) = @_;
   if ($key && $declHash{$key})
   {   return $declHash{$key}->file()->longname();   }
#   else
#   {   $declName = " ";  }
}

# return name of declaration entity or null if none
# used for sorting by decl name
sub getDeclName
{
   my ($key) = @_;
   if ($key && $declHash{$key})
   {   return $declHash{$key}->ent()->name();   }
#   else
#   {   $declName = " ";  }
}

# return declaration ref (based on language) or 0 if unknown
sub getDeclRef 
{
   my ($ent) =@_;
   my @decl=();
   return @decl unless defined ($ent);
   
   if ($language =~ /ada/i)
   {
      my @declOrder = ("declarein ~spec ~body ~instance ~formal ~incomplete ~private ~stub",
                 "spec declarein",
                 "body declarein",
                 "instance declarein",
                 "formal declarein",
                 "incomplete declarein",
                 "private declarein",
                 "stub declarein");
               
      foreach my $type (@declOrder)
      {
         @decl = $ent->refs($type);
         if (@decl) { last;  }
      }
   }
   elsif ($language =~ /fortran/i)
   {
      @decl = $ent->refs("definein");
      if (!@decl) {   @decl = $ent->refs("declarein");  }
   }
   else # C/C++
   {
      @decl = $ent->refs("definein");
      if (!@decl) {   @decl = $ent->refs("declarein"); }
   }
   
   if (@decl)
   {   return $decl[0];   }
   else
   {   return 0;   }
}

# load matching Ada entities, ignoring any standard lib entities
# match is case-insensitive
sub loadAdaEnts
{
   my ($pattern) = @_;
   foreach my $ent ($db->ents())
   {
      # ignore standard lib entities
      if (not ($ent->library("Standard") ) )
      {
         if ( ($ent->name() =~ /$pattern/i) )  # case-insensitve match
         {
            my $declRef = getDeclRef($ent);
            next if ($kindfilter && ($ent->kindname() !~ /$kindfilter/i) );
            if ($declfilter)
            {
               next if (not $declRef);
               next if ($declRef->ent()->longname() !~ /$declfilter/i);
            }
            if ($filefilter)
            {
               next if (not $declRef);
               next if ($declRef->file()->longname() !~ /$filefilter/i);
            }
            # passed all the possible filter tests, load the entity and declaration 
            $entHash{$ent->id()} = $ent;
            $declHash{$ent->id()} = $declRef;
         }
      }
   }
}

# load matching C entities, 
# match is case-sensitive
sub loadCEnts
{
   my ($pattern) = @_;
   foreach my $ent ($db->ents())
   {
      if ( ($ent->name() =~ /$pattern/) )  # case-sensitve match
      {
         my $declRef = getDeclRef($ent);
         next if ($kindfilter && ($ent->kindname() !~ /$kindfilter/i) );
         if ($declfilter)
         {
            next if (not $declRef);
            next if ($declRef->ent()->longname() !~ /$declfilter/);
         }
         if ($filefilter)
         {
            next if (not $declRef);
            next if ($declRef->file()->longname() !~ /$filefilter/);
         }
         # passed all the possible filter tests, load the entity and declaration 
         $entHash{$ent->id()} = $ent;
         $declHash{$ent->id()} = $declRef;
      }
   }
}

# load matching Fortran entities, 
# match is case-insensitive
sub loadFortranEnts
{
   my ($pattern) = @_;
   foreach my $ent ($db->ents())
   {
      if ( ($ent->name() =~ /$pattern/i) )  # case-insensitve match
      {
         next if ($kindfilter && ($ent->kindname() !~ /$kindfilter/i) );
         my $declRef = getDeclRef($ent);
         if ($declfilter)
         {
            next if (not $declRef);
            next if ($declRef->ent()->longname() !~ /$declfilter/i);
         }
         if ($filefilter)
         {
            next if (not $declRef);
            next if ($declRef->file()->longname() !~ /$filefilter/i);
         }
         # passed all the possible filter tests, load the entity and declaration 
         $entHash{$ent->id()} = $ent;
         $declHash{$ent->id()} = $declRef;
      }
   }
}

# print entity info: entity short name, entity kind, declaration file and line 
sub printData
{
    my ($key) = @_;
 
    printf "%-*s %-*s ",
       $min_name_width,
       $entHash{$key}->name(),
       $min_kind_width,
       $entHash{$key}->kindname();
       
    if ($declHash{$key})
    {
       printf "%-*s File: %s Line: %d",
          $min_decl_width,
          $declHash{$key}->ent()->name(),
          $declHash{$key}->file()->longname(),
          $declHash{$key}->line();
    }
    print "\n";   
    if ($comments)
    { 
        if ($entHash{$key}->comments())
        {    
           print "//////\n";
           print $entHash{$key}->comments()."\n"; 
           print "//////\n\n";
        }
    }
}

# sort by declaration entity  name
sub reportDeclOrder
{
   foreach my $key (sort { lc(getDeclName($a)) cmp lc(getDeclName($b)); } keys (%declHash))
   {
      printData ($key);
   }
}

# sort by declaration file name
sub reportFileOrder
{
   foreach my $key (sort { lc(getDeclFileName($a)) cmp lc(getDeclFileName($b)); } keys (%declHash))
   {
      printData ($key);
   }
}

# sort by entity Kind
sub reportKindOrder
{
   foreach my $key (sort { $entHash{$a}->kindname() cmp $entHash{$b}->kindname; } keys (%entHash))
   {
      printData ($key);
   }
}

# sort by entity (short) name
sub reportNameOrder
{
   foreach my $key (sort { lc($entHash{$a}->name()) cmp lc($entHash{$b}->name); } keys (%entHash))
   {
      printData ($key);
   }
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

