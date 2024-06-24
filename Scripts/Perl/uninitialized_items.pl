#
# Sample Understand perl API program 
#
# Synopsis: Return a list of all uninitialized items similar to that created in the Understand html report
#
# Categories: Project Report
#
# Languages: All
#
# Uninitialized Items
#
# Usage:
sub usage($) {
    return shift(@_) . <<"END_USAGE";
Usage: $0 -db database
  -db database      Specify Understand database (required for
                    uperl, inherited from Understand)
END_USAGE
}
#
#  For the latest Understand perl API documentation, see
#      http://www.scitools.com/perl.html
#
#  18-Mar-2011 KG

use Understand;
use Getopt::Long;
use strict;

my $dbPath;
my $help;
GetOptions(
	   "db=s" => \$dbPath,
	   "help" => \$help,
          );

# help message
die usage("") if ($help);

# open the database
my $db=openDatabase($dbPath);



#code body*******************************************************************
my @objs = $db->ents("ada object ~loop ~task,".
           "c# variable,".
           "c object ~unknown ~unresolved,".
           "fortran variable,".
           "java variable,".
           "jovial variable ~table ~block,".
           "pascal variable,".
           "plm variable");
foreach my $obj(@objs){
  next if $obj->library;
  my $ref = getDeclRef($obj);
  next unless $ref;
  my $parent = $ref->ent;
  next if $parent->kind->check("c struct, c union");
  next if $obj->ref("setby init ~implicit");
  print $ref->file->relname."(".$ref->line.")".$ref->scope->longname."\n";
}

#end body********************************************************************
closeDatabase($db);


# subroutines

# return declaration ref (based on language) or 0 if unknown

sub getDeclRef
{
   my ($ent) =@_;
   my @decl=();
   return @decl unless defined ($ent);
   my $language = $ent->language;
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
    $db->close() if $dbPath;
}

# Pass in percent complete (decimal value) to update a progress bar in the GUI or command line
sub progress{
	my $percent = shift;
	if(Understand::Gui::db()){
		Understand::Gui::progress_bar($percent);
		Understand::Gui::yield();
	}else{
		print "Progress: ".int($percent*100)."%\r";
	}
}
