# a_duplicate_code.pl - sample Understand perl API program
#
# Synopsis: Report duplicate code.
#
# Categories: Project Report, Coding Standards
#
# Languages: Ada
#
# Usage:
sub usage
{
    return << "END_USAGE";
${ \( shift @_ ) }
usage: a_duplicate_code  -db database\n
 -db databse       Specify Understand database, if run from uperl.
END_USAGE
}

use strict;
use Understand;
use Getopt::Long;
use File::Basename;
use IO::File;
use POSIX qw(tmpnam);
use Env;
use File::Find;
use Tie::RefHash;

my $argHelp;
my $argDb;
my $db;
tie my %checksum_hash, 'Tie::RefHash';

my @files = ();

GetOptions(
	   "db=s" => \$argDb,
	   "help" => \$argHelp,
           );


# do help option
die usage("") if $argHelp;

$db = openDatabase($argDb);

# read files 
readDatabase($db,\@files);

print "Calculating checksums\n";
foreach my $f_ent (@files) {
   my %prog_unit_hash = ();

   # get all declared units (including nested ones) that have a
   #  context - want package spec and body, subroutine bodies
   getProgUnits($f_ent, $f_ent, \%prog_unit_hash);
   # calculate checksums for units in current file
   calcUnitChecksums($f_ent, \%prog_unit_hash, \%checksum_hash);
}

printDuplicateInfo(\%checksum_hash);

closeDatabase($db);


# subroutines 


sub getProgUnits
{
   my ($c_ent, $file_ent, $pu_hash_ref) = @_;    

   my @prog_unit_refs = $c_ent->refs("declare spec", 
        "task, protected, package");
   
   foreach my $prog_ref (@prog_unit_refs) {
      next if $prog_ref->file()->id() != $file_ent->id();
      $pu_hash_ref->{$prog_ref->line()} = $prog_ref;
      getProgUnits($prog_ref->ent(), $file_ent, $pu_hash_ref);
   }

   my @prog_unit_refs = $c_ent->refs("declare body ~stub", 
        "task, protected, package, function, procedure");
   
   foreach my $prog_ref (@prog_unit_refs) {
      next if $prog_ref->file()->id() != $file_ent->id();
      $pu_hash_ref->{$prog_ref->line()} = $prog_ref;
      getProgUnits($prog_ref->ent(), $file_ent, $pu_hash_ref);
   }
}


sub readDatabase
{
   my ($db,$file_array_ref) = @_;    

   print "Reading Understand database\n";

   my $language = $db->language();

   # check language
   if ( $db->language() !~ "Ada" ) {
      die "This script is designed for Ada only\n";
   }
   my @allfiles = $db->ents("file ~unresolved");
   foreach my $file (@allfiles) {
      if ($file->library() ne "Standard") {
        push @$file_array_ref, $file;
      }
   }
}

sub calcUnitChecksums
{
   my ($file, $pu_hash_ref, $checksum_hash_ref) = @_; 

   # create lexer object
   my ($lexer,$status) = $file->lexer();
   if ($status) {
      print "file $status, cannot examine data\n";
      return;
   }

   foreach my $pi (sort keys %$pu_hash_ref) {
      my $prog_ref = $pu_hash_ref->{$pi};
#debug
#print $prog_ref->ent()->name(). " " . $prog_ref->file()->name() . "\n";
      my ($fingerprint, $end_line, $end_column) = 
                           get_fingerprint($lexer, $prog_ref); 
     if ($fingerprint) {
        if (!{$checksum_hash_ref->{$fingerprint}}) {
            @{$checksum_hash_ref->{$fingerprint}} = ();
         }
         push @{$checksum_hash_ref->{$fingerprint}}, $prog_ref;
      }
   }
}

sub printDuplicateInfo 
{
   my ($checksum_hash_ref) = @_; 

   print "\n" . "Duplicate units:\n";
   foreach my $fp (keys %$checksum_hash_ref) {
      if ($#{$checksum_hash_ref->{$fp}} > 0) {
         foreach my $pu_ref (@{$checksum_hash_ref->{$fp}}) {
            print $pu_ref->ent()->longname() . " File: " . 
                $pu_ref->file()->name() . " Line: " . $pu_ref->line() . "\n";
         }
         print "\n";
      }
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



sub get_fingerprint()
{
   my ($lexer,$ref) = @_;

   #find appropriate end_ref
   my $unit_start_line = $ref->line();
#debug
#print "start line: $unit_start_line\n";
   my (@end_refs) = $ref->ent()->refs("End");
   my $end_ref;
   foreach my $er (@end_refs) {
      next if ($er->file() != $ref->file());
      if (!$end_ref) {
         $end_ref = $er;
      }
      elsif ($end_ref->line() < $unit_start_line) {
         $end_ref = $er;
      }
      elsif ($er->line() > $unit_start_line &&
          $er->line() < $end_ref->line()) {
         $end_ref = $er;
      }
   }
#debug
#if ($end_ref) {
#   print "end line: " . $end_ref->line() . "\n";
#}
#else {
#   print "NO END LINE\n";
#}
   if ($end_ref) {
      my $end_named = $end_ref->kind()->check("end ~unnamed");
      my $string;
      my $lexeme = $lexer->lexeme($ref->line(),$ref->column());
      # skip the routine name so copied routine with different name
      #   will match
      if ($lexeme)  {
	 $lexeme=$lexeme->next;
      }
      my $in_whitespace = 0;
      while($lexeme) { 
	    
         # for comments/whitespace, add a single blank
         if ( $lexeme->token() =~ "Comment" ||
              $lexeme->token() =~ "Whitespace" ||
              $lexeme->token() =~ "Newline") {
            if (!$in_whitespace) {
               $string = $string . " ";
               $in_whitespace = 1;
            }
         }
         else {
              if (!$end_named ||
                  ($lexeme->line_end != $end_ref->line() ||
                   $lexeme->column_end < $end_ref->column())) { 
                 $string = $string . $lexeme->text();
                 $in_whitespace = 0;
              }
	 }
	 # break when it's line/column numbers match the last one
	 if ( $lexeme->line_end == $end_ref->line() &&
              $lexeme->column_end >= $end_ref->column()) {
            last;
	 }
	    
	 # increment to next one
	 $lexeme=$lexeme->next;
      }
      if ($string eq " is end" ||
         $string =~ /\A renames /) {
         return(undef,undef,undef);
      }
      else {
         return(Understand::Util::checksum($string),$end_ref->line(),$end_ref->column());
      }
   }
   else {
      return(undef,undef,undef);
   }
}










