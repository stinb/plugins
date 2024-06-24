#
# Sample Understand perl API program 
#
# Synopsis: Saves graphics pictures for items specified in an input file.
#
# Categories: Graphic, Entity Report
#
# Languages: Ada, Fortran, C
#
# Usage:
sub usage ($)
{
    return << "END_USAGE";
${ \( shift @_ ) }
Usage: picdumper.pl -db database -file inputfile [-out directory] [-showfile] [-verbose]
 -db database     (required) specify an Understand for C database.
 -file inputfile  (required) specify the name of the file to be read for input.
                  The file is to be of the following format, with one line per
                  graphics drawing:
  entity name | view | graphic options | output filename (includes extension) | variant
                  Each field is delimted by a vertical bar (|).  Any amount of 
                  whitespace (spaces or tabs may occur between fields.  
                  The # sign may be used for comment lines.   Refer to
                  the perl API documentation located at
                  http://www.scitools.com/perl/Understand.html  for the
                  current list of view types and graphic options available.
   for example:
#  entity name | view       | graphic options    | graphic filename
    myFunc     | Invocation | level=4;scale=14pt | myFunc_inv.png

 -out directory   (optional) Specify the directory path where graphic files
                  are to be written.  If not specified, the current directory
                  is used.  The directory path specified must already exist.
 -showfile        (optional) If specified, the full path of the file where the
                  entity is defined will be shown in the index file.
 -verbose         (optional) When specified, a message will be output to STDOUT
                  for each graphic file written.
END_USAGE
}
# Description:
#  Reads an input file which specifies entity name, graphic view, graphic  
#  options, and graphic file name to output and calls ent->draw() for each 
#  line item in input file. An HTML Table of Contents is also generated,  
#  index.html, and resides in the directory containing the graphics files.
#  The input file is to be of the following format, with one line per graphics drawing:
#      entity name | view | graphic options | output filename (includes extension) | variant
#  Each field is delimted by a vertical bar (|).  Any amount of whitespace (spaces or tabs
#  may occur between fields.  The # sign may be used for comment lines. 
#  Refer to the perl API documentation located at
#        http://www.scitools.com/perl/Understand.html  for the
#  current list of view types and graphic options available.
# 
#  Requires an existing Understand database.
#
#  For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl/Understand.html
#  Refer to the documenation for details on using the perl API 
#  with Understand and for details on all Understand perl API calls.
# 
#  03-Jan-2001 TLB
#  19-Feb-2001 TLB Added HTML Table of Contents
#  05-Aug-2001 DLL - updated for Understand::Gui::db()
#
use Understand;
use Getopt::Long;
use strict;

my $dbPath;
my $inputfile;
my $dirpath;
my $showfile;
my $verbose;
my $help;
GetOptions(
	   "db=s"     => \$dbPath,
           "file=s"   => \$inputfile,
           "out=s"    => \$dirpath,
           "showfile" => \$showfile,
           "verbose"  => \$verbose,
	   "help" => \$help,
           );

# help message
die usage("") if ($help);
                  
# required args
die usage("No input file.\n") unless ($inputfile);

# open the database 
my $db=openDatabase($dbPath);

my $language = $db->language();

open (INFILE, $inputfile) || die "Can't open $inputfile: $! \n";

# use current dir if -out arg not specified
$dirpath ||= ".";

my $total = 0;
my %entHash =();
my @allents = $db->ents("~unresolved ~unknown");  

while (<INFILE>)
{
   chomp;
   s/#.*//;                     # remove comments
   next unless ($_);            # skip blank lines
   my ($entname, $view, $options, $outfile, $variant) = split(/\s*\|\x*/);
   $entname =~ s/^\s*//;        # remove any leading whitespace
   $entname =~ s/\s*$//;        # remove any trailing whitespace
   $view =~ s/^\s*//;        # remove any leading whitespace
   $view =~ s/\s*$//;        # remove any trailing whitespace
   $options =~ s/^\s*//;        # remove any leading whitespace
   $options =~ s/\s*$//;        # remove any trailing whitespace
   $outfile =~ s/^\s*//;        # remove any leading whitespace
   $outfile =~ s/\s*$//;        # remove any trailing whitespace
   $variant =~ s/^\s*//;        # remove any leading whitespace
   $variant =~ s/\s*$//;        # remove any trailing whitespace
   print "entname=$entname, view=$view, options=$options, outfile=$outfile\n";

   my @ents = lookupEnt ($entname);
   my $count=0;                    # handle possibility of multiple entities of same name
   foreach my $ent (@ents)
   {  
       my $filename;
       if ($count) { $filename = $dirpath."/".$count."_".$outfile }
       else { $filename = $dirpath."/".$outfile; }
       print "writing $view for ".$ent->longname()." to $filename \n" if $verbose;
       my $status = $ent->draw($view, $filename, $options, $variant);
       if ($status)
       { print "Error writing \"$filename\" (".$ent->name()."): $status\n"}
       else
       { 
	   if (not $entHash{$ent->id()})  
	   {
	       $entHash {$ent->id()} {Shortname} = $ent->name();    #same for each view for this entity
	       if ($showfile)
	       {
		   if ($ent->kind()->check("file") )
		   {  $entHash {$ent->id()}{DeclFile} = $ent->longname(); }
		   else
		   {  $entHash {$ent->id()} {DeclFile} = (getDeclRef($ent))->ent()->longname(); }
	       }
	       else  
	       {  $entHash {$ent->id()}{DeclFile} = 0;  }
	   }
	   $entHash {$ent->id} {$view} = $outfile;
	   $count++; 
       }
   }
   $total += @ents;
}

close (INFILE);

# write out html table of contents
writeTOC ($dirpath);

print "$total graphic files for ".scalar(keys %entHash)." entities written to $dirpath\n";

closeDatabase($db);

# subroutines


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

sub lookupEnt 
{ 
   my($entname) = @_;
   my @entlist=();
   
   return () unless $entname;
 
   foreach my $ent (@allents)
   {
      if ( ($entname eq $ent->name()) ||
           ($entname eq $ent->longname()) )
      {  push @entlist, $ent;   }

    }  
    return @entlist;
}

sub writeTOC
{
   my ($path) = @_;
   my ($name, $file);        # makes for easier reading of hash variables
   
   # create main frame 
   open (INDEX, ">".$path."/index.html") || die "Can't create index.html: $! \n";
   print INDEX "<HTML><HEAD><TITLE>".$db->name()."</TITLE>\n";
   print INDEX "</HEAD>\n";
   print INDEX "<FRAMESET COLS=\"20%,*\">\n";
   print INDEX "<FRAME SRC=\"toc.html\" NAME=\"toc\">\n";
   print INDEX "<FRAME SRC=\"startview.html\" NAME=\"view\">\n";
   print INDEX "</FRAMESET>\n";
   print INDEX "</HTML>\n";
   close (INDEX);
   
   # create opening view 
   open (STARTVIEW, ">".$path."/startview.html") || die "Can't create startview.html: $! \n";
   print STARTVIEW " <HTML><HEAD></HEAD><BODY>\n";
   print STARTVIEW "<BR><BR><BR>Select an entity view from the list to the left";
   print STARTVIEW "</BODY></HTML>\n";
   close (STARTVIEW);
   
   # now write out the table of contents doc
   open (TOC, ">".$path."/toc.html") || die "Can't create table of contents file: $! \n";
   # write header of TOC file
   print TOC "<HTML><HEAD><TITLE>Table of Contents</TITLE>\n</HEAD>\n<BODY>\n";
   print TOC "<TABLE COLS=3>\n";
   
   # sort by entity name and write to Table of Contents
   foreach my $eId (sort {lc($entHash{$a}{Shortname}) cmp lc($entHash{$b}{Shortname}); } keys %entHash)
   {
      $name = $entHash{$eId} {Shortname};
      print TOC "<TR>";
      print TOC "<TD COLSPAN=3>$name";
      if ($entHash{$eId}{DeclFile})
      {  print TOC " (".$entHash{$eId}{DeclFile}.")";   }
      print TOC "<TD></TR>\n";
      # sort by view name
      foreach my $view ( sort {lc($entHash{$eId}{$a}) cmp lc($entHash{$eId}{$b}); } keys %{$entHash{$eId}} )    
      {
         if (($view eq "Shortname") || ($view eq "DeclFile")) { next; }
         $file = $entHash{$eId}{$view};
         print TOC "<TR><TD></TD><TD></TD><TD NOWRAP>";
         print TOC "<A HREF=\"$file\" TARGET=view onClick=\'window.status=\"$view : $name \"; parent.document.title=\"$view : $name \"; return true;\'>$view";
         print TOC "</TD></TR>\n";
      }
   }

   # close TOC file
   print TOC "</TABLE>\n</BODY></HTML>";
   close (TOC);

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



