#
# Synopsis: Create invocation graphics files
#
# Categories: Graphic, Project Report
#
# Languages: All
#
# Usage:
sub usage ($)
{
    return << "END_USAGE";
${ \( shift @_ ) }
Usage: invtree.pl -db database [-name pattern]
    -db database     Specify Understand database (required for
                     uperl, inherited from Understand)
    -name pattern    (optional) Limit graphs to entities that
                     match given pattern
END_USAGE
}

#  Creates a directory whose name matches that of the database and stuffs it 
#  full of PNG files of the invocation trees of all things with call trees 
#  in the database that match the optional name pattern.
#
#  Ada, C or FORTRAN.
#
#  Designed as a GUI base script. 'invtrees' directory is relative to the CWD
#  as specified from the run a script dialog (or current Understand CWD if not
#  specified).
#
#  Desired: An index.html file with a frameset TOC (for viewing) (like 
#           picdumper.pl does), with a resulting start of index.html to 
#           load the browser
#

my $graphOptions="name=shortname";
my $variant="Simplified";
use Understand;
use Getopt::Long;
use strict;
use File::Basename;

my $dbPath;
my $name;
my $help;
GetOptions(
	   "db=s"    => \$dbPath,
       "name=s"  => \$name,
	   "help"    => \$help,
           );

# help message
die usage("") if ($help);
                  
# open the database 
my $db=openDatabase($dbPath);

# get the language
my $language = $db->language();
print "language = $language\n";

# directory name to put files into
my $dirname = basename($db->name());
$dirname =~ s/\.ud.$//;
$dirname = $dirname . "_invtree";

# create the directory if necessary
mkdir($dirname,0777) unless (-d $dirname);

# dir separator
my $slash = "/";

# default to all graphics
$name = "*" if !$name;

# replace wildcards * and ? with regex wildcards .* and ., respectively
$name =~ s/\*/\.\*/g; 
$name=~ s/\?/\./g;   

my %graphics = ();
my $unk = "~unknown ~unresolved";
my @ents = $db->ents("ada entry $unk,ada function $unk,ada package $unk,ada procedure $unk,ada protected $unk,ada task $unk,"
."c function $unk,"
."csharp method $unk,"
."fortran block data $unk, fortran function $unk, fortran interface $unk, fortran program $unk, fortran subroutine $unk,"
."java method $unk,"
."jovial file $unk, jovial subroutine $unk,"
."pascal function $unk, pascal procedure $unk, pascal compunit $unk,"
."plm procedure $unk,"
."vhdl architecture $unk, vhdl function $unk, vhdl process $unk, vhdl procedure");

print @ents ." ents";
foreach my $ent (sort{$a->longname() cmp $b->longname();} @ents) {

    next if ( $ent->name() !~ $name );
    next if $ent->library();
    my $pngname = makePngName( $ent );
    my $file = $dirname . $slash . $pngname;
    my $status = $ent->draw("Calls",$file,$graphOptions,$variant);
    if ( !$status ) {
	print "graphic $file created\n";
	$graphics{$ent->id()}{"ent"} = $ent; 
	$graphics{$ent->id()}{"pngname"} = $pngname; 
    }
	else{
		print $status."\n";
	}
}

# create HTML page with links to all 
my $index = writeTOC($dirname,%graphics);

# tell user how to view it
tellUser($index);

# close 
closeDatabase($db);


# subroutines


sub tellUser($)
{
    my ($index) = @_;

    my $linenum=1;
    print "\nTo view graphics browse to: \n";
    print "     $index \n";
    #print "[file: ",$index," line: ",1,"]\n";
}



my %pngnames = ();
sub makePngName($)
{
    my ($ent) = @_;

    # create the name
    my $name = $ent->longname() . ".png";

    # remember how many occurances of this name 
    $pngnames{$name}++; 

    # recreate name with trailing _N if more than one match 
    if ( $pngnames{$name} > 1) {
	my $n = $pngnames{$name} - 1;
	$name = $ent->longname() . "_" . $n . ".png";
    }

    return($name);
}

sub writeTOC($$)
{
   my ($dirname,$graphics) = @_;

   my ($name, $file);        # makes for easier reading of hash variables
   
   my $index = $dirname . $slash . "index.html";
   my $startview = $dirname . $slash . "startview.html";
   my $toc = $dirname . $slash . "toc.html";


   #print "index=$index\n";
   #print "startview=$startview\n";
   #print "toc=$toc\n";

   open (INDEX, ">".$index) || die "Can't create index.html: $! \n";
   print INDEX "<HTML><HEAD><TITLE>".$dirname."</TITLE>\n";
   print INDEX "</HEAD>\n";
   print INDEX "<FRAMESET COLS=\"20%,*\">\n";
   print INDEX "<FRAME SRC=\"toc.html\" NAME=\"toc\">\n";
   print INDEX "<FRAME SRC=\"startview.html\" NAME=\"view\">\n";
   print INDEX "</FRAMESET>\n";
   print INDEX "</HTML>\n";
   close (INDEX);
   
   # create opening view 
   open (STARTVIEW, ">".$startview) || die "Can't create startview.html: $! \n";
   print STARTVIEW " <HTML><HEAD></HEAD><BODY>\n";
   print STARTVIEW "<BR><BR><BR>Select an entity view from the list to the left";
   print STARTVIEW "</BODY></HTML>\n";
   close (STARTVIEW);
   
   # now write out the table of contents doc
   open (TOC, ">".$toc) || die "Can't create table of contents file: $! \n";
   # write header of TOC file
   print TOC "<HTML><HEAD><TITLE>Table of Contents</TITLE>\n</HEAD>\n<BODY>\n";
   print TOC "<TABLE COLS=3>\n";
   
   # sorted by entity names and write to Table of Contents
   foreach my $key (sort {lc($graphics{$a}{"ent"}->name()) cmp lc($graphics{$b}{"ent"}->name()); } keys %graphics)
   {
       my $ent = $graphics{$key}{"ent"};
       my $name = $ent->name();
       #my $name = $graphics{$key}{"pngname"};
       print TOC "<TR>";
       print TOC "<TD COLSPAN=3>$name";
       my $declRef = getDeclRef($ent);
       if ($declRef) {
	   my $declFile = $declRef->ent()->longname();
	   if ($declFile) {
	       print TOC " (".$declFile.")";
	   }
       }
       else {
	   print "no declaration ref for $name\n";
       }
       print TOC "<TD></TR>\n";
       # sort by view name
       #foreach my $view ( sort {lc($entHash{$eId}{$a}) cmp lc($entHash{$eId}{$b}); } keys %{$entHash{$eId}} )    
       #{
	   #if (($view eq "Shortname") || ($view eq "DeclFile")) { next; }
           #my $file = $ent->longname() . ".png";
           my $file = $graphics{$key}{"pngname"};
           my $view = "Invocation";
	   print TOC "<TR><TD></TD><TD></TD><TD NOWRAP>";
	   print TOC "<A HREF=\"$file\" TARGET=view onClick=\'window.status=\"$view : $name \"; parent.document.title=\"$view : $name \"; return true;\'>$view";
	   print TOC "</TD></TR>\n";
       #}
   }

   # close TOC file
   print TOC "</TABLE>\n</BODY></HTML>";
   close (TOC);

   return($index);
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



