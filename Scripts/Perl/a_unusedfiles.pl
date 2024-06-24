#
# Synopsis: Reports unused files
#
# Categories: Project Report, Coding Standards
#
# Languages: Ada
#
# Description:
#   Reports all files that are composed entirely of things referred to only by compilation
#   units declared in that file. 
#
#   For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl/
#

sub usage($) {
    print shift,"\n";
    print "Usage: a_unusedfiles.pl\n";
    print "  -db   database    Specify database, if used with uperl.\n";
}


use Understand;
use Getopt::Long;
use strict;

# get options
my ($context,$dbPath,$help);

GetOptions
    (
     "db=s"      => \$dbPath,
     "help"      => \$help,
     ) or die usage("");
die usage("") if ($help);

# open the database
my $db=Understand::Gui::db();
if (!$db) {
    die usage("Error, database not specified") unless $dbPath;
    my $status;
    ($db,$status)=Understand::open($dbPath);
    die print "Error opening database: $status\n" if $status;
}


# check language
my $language = $db->language();
die print "$language is currently unsupported" if ($language !~ /ada/i);
 

# set debug printing on/off
my $printDebugs = 0;

# Get all files

my $numFound = 0;

my @files = $db->ents("file");
foreach my $file (sort {$a->longname() cmp $b->longname()} @files)
{

 # skip standard files
 
 if ($file->library() ne "Standard")
 {

   print "------- File:" . $file->longname() . "\n" if ($printDebugs);
   
   # start up the lexer for this file. We will walk through it looking for
   # things being declared. When found, check for external references to it.
 
    my $lexer = $file->lexer();
 
    my $foundExternal = 0;
    my $lexeme = $lexer->first();
  
    while ($lexeme && ! $foundExternal)
    {
  
      # if this lexeme is a reference, check to see if it is declaring something
      # This being Ada, there are a few ways to "declare" things. The list below covers
      # them.
      
      if ($lexeme->ref())
      {
     
       print "Checking\t...... " . $lexeme->ent()->longname() . "   " . $lexeme->ref()->kindname() . "\n" if ($printDebugs);
     
       # check to see if this reference is a declaration.
       # if so, check to see if any of its references come outside this file or compilation unit.   

        if ($lexeme->ref()->kindname() eq "Declare" || 
                        $lexeme->ref()->kindname() eq "Instantiation" ||
                        $lexeme->ref()->kindname() eq "Child" ||
                        $lexeme->ref()->kindname() eq "Spec" || 
		        $lexeme->ref()->kindname() eq "Body" 
            
           )
        {
   
         # Now check for any references of this entity outside of this file or this compilation unit
         
         my @refs;
         
         @refs = $lexeme->ent()->refs("callby, withby, useby, setby");
          
         if ( @refs )
         {
          my $curRef = 0;
          while (@refs->[$curRef] && ! $foundExternal)
          {

           print "..................Checking ref " . @refs->[$curRef]->kindname() . " " . @refs->[$curRef]->ent()->ref("declarein")->file()->name() . "\n" if ($printDebugs);          
           if (my $declaredIn = @refs->[$curRef]->ent()->ref("declarein"))
           {
             print $declaredIn->ent()->longname() . " " . $lexeme->ent()->longname() . "\n" if ($printDebugs);      
             if ( (@refs->[$curRef]->file()->longname() != $file->longname()) ||
                  (@refs->[$curRef]->file()->longname() == $file->longname()) &&
                   ($declaredIn->ent()->longname() ne $lexeme->ent()->longname()) )
             {
               $foundExternal = 1;  
               print "*\n" if ($printDebugs);
             }
           }       
           $curRef++;
          }
                 
         }
        }
  
      }
      $lexeme=$lexeme->next();
    }
    
    if (! $foundExternal)
    {
       print "Unused File: " . $file->longname() . "\n";
       $numFound++;
    }
     
   
  }
}

print "\nFound " . $numFound . " unused files.\n";

