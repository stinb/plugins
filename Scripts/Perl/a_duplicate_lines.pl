# a_duplicate_lines.pl
#
# Synopsis: Report duplicate lines
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
usage: a_duplicate_lines  -db database\n
 -db database       Specify Understand database, if run from uperl.
 -n                 Number of lines that much match before being reported
                       (default value is 5).
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

my $argHelp;
my $argDb;
my $argN = 5;
my %checksum_hash;
my %blankline_hash;
my $db;

my @files = ();

GetOptions(
	   "db=s" => \$argDb,
	   "help" => \$argHelp,
           "n=f" => \$argN,
           );


# do help option
die usage("") if $argHelp;

$db = openDatabase($argDb);

# read files 
readDatabase($db,\@files);

print "Calculating checksums\n";
foreach my $f_ent (@files) {
   # calculate checksums for lines in current file
   calcLineChecksums($f_ent, \%checksum_hash, \%blankline_hash);
}

printDuplicateInfo(\%checksum_hash, \%blankline_hash);

closeDatabase($db);

# subroutines 


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

sub calcLineChecksums
{
   my ($file, $checksum_hash_ref, $blankline_hash_ref) = @_; 
   # checksum_hash_ref is ref to hash of checksum for arrays of 
   #  file/line records
   # blankline_hash_ref is ref to hash of file for hash of blank line numbers
   my %lexeme;

   # debug
   # print "\n" . $file->name() . "\n";
   # create lexer object
   my ($lexer,$status) = $file->lexer();

   if ($status) {
      print "file $status, cannot examine data\n";
      return;
   }

   my $lexeme = $lexer->first();

   while ($lexeme)
   {
      my $fingerprint;
      my $line = $lexeme->line_begin();
      ($fingerprint, $lexeme) = 
                           getLineFingerprint($lexeme); 
      if ($fingerprint) {
         # add to hash checksum->file/line array
        my $rec = {FILE => $file, LINE => $line};
        if (!{$checksum_hash_ref->{$fingerprint}}) {
            @{$checksum_hash_ref->{$fingerprint}} = ();
         }
         push @{$checksum_hash_ref->{$fingerprint}}, $rec;
      }
      else {
         # add to hash blanklines, file/line->1
         if (!$blankline_hash_ref->{$file->id()}) {
            $blankline_hash_ref->{$file->id()} = {};
         }
         $blankline_hash_ref->{$file->id()}{$line} = 1;
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
    $db->close() if (!Understand::Gui::active());
}



sub getLineFingerprint
{
   my ($lexeme) = @_;
   my $string; 
   my $in_whitespace = 0;
   # skip over leading white space
   while ($lexeme && ($lexeme->token() =~ "Whitespace")) {
      $lexeme = $lexeme->next;
   }
   while ($lexeme && ($lexeme->token() !~ "Newline")) {
      # for comments/whitespace, add a single blank
      if ($lexeme->token() =~ "Whitespace") {
         if (!$in_whitespace) {
            $string = $string . " ";
            $in_whitespace = 1;
         }
      }
      elsif ($lexeme->token() =~ "Comment") {
         # just skip over the comment
      } 
      else {
           $string = $string . $lexeme->text();
           $in_whitespace = 0;
      }
      $lexeme = $lexeme->next;
   }
   # advance past new-line
   if ($lexeme) {
      $lexeme = $lexeme->next;
   }
   # strip trailing blank
   $string =~ s/ $//;
   # force string to lower case
   $string = "\L$string\E";
   if ($string && $string ne " ") {
      return(Understand::Util::checksum($string), $lexeme);
   }
   else {
      return(undef, $lexeme);
   }
}

sub numerically {$a <=> $b}

sub printDuplicateInfo
{
   my ($checksum_hash_ref, $blankline_hash_ref) = @_;
   my %dup_fileline_hash;
      # hash of file to hash of line to checksum
   my %done_file_hash = {};

   foreach my $fp (keys %$checksum_hash_ref) {
      my @file_line_array = @{$checksum_hash_ref->{$fp}};
      # if there were duplicates for the current checksum
      if ($#file_line_array > 0) {
         foreach my $rec (@file_line_array) {
            my $r_file = $rec->{FILE};
            my $r_line = $rec->{LINE};
            if (!$dup_fileline_hash{$r_file->id()}) {
               $dup_fileline_hash{$r_file->id()} = {};
            }
            $dup_fileline_hash{$r_file->id()}{$r_line} = $fp;
         }
      }
   }
# debug
#   print "Blank lines:\n";
#   foreach my $fid (keys %$blankline_hash_ref) {
#      my $c_file = $db->ent_from_id($fid); 
#      my %line_hash = %{$blankline_hash_ref->{$fid}};
#      print "   File: " . $c_file->name() . "\n";
#      foreach my $c_line (keys %line_hash) {
#         print $c_line . "\n";
#      }
#   }
   foreach my $file_id (keys %dup_fileline_hash) {
      my $file = $db->ent_from_id($file_id);
      my %done_line_hash = {};
      foreach my $line (sort numerically keys %{$dup_fileline_hash{$file_id}}) {
         my $fp = $dup_fileline_hash{$file_id}{$line};
         my @file_line_array = @{$checksum_hash_ref->{$fp}};
         foreach my $rec (@file_line_array) {
            my $match_file = $rec->{FILE};
            my $match_line = $rec->{LINE};
            my $cur_orig_line = $line + 1;
            my $cur_match_line = $match_line + 1;
            my $m_count = 1;
            my $matches = 1;
            my $last_line = $line;
            my $last_match_line = $match_line;
            # skip record if this is the current file/line
            next if ($match_line == $line &&
                     $match_file->id() == $file_id);
            # skip record if the file has already been done
            next if (%done_file_hash->{$match_file->id()});
            # skip record if this file/line has already been reported
            #   as part of another sequence
            if (!%done_line_hash->{$match_file->id()}) {
               %done_line_hash->{$match_file->id()} = {};
            }
            next if (%done_line_hash->{$match_file->id()}{$line,$match_line});
            %done_line_hash->{$match_file->id()}
                 {$line,$match_line} = 1;
            if ($match_file->id() == $file_id) {
              %done_line_hash->{$match_file->id()}
                 {$match_line,$line} = 1;
            }
            while ($matches) {
               my $cur_orig_fp = %dup_fileline_hash->{$file_id}{$cur_orig_line};
               my $cur_match_fp = %dup_fileline_hash->{$match_file->id()}
                                                       {$cur_match_line};
               # stop if both code segments are in the same file and
               #   the current match line has caught up with the
               #   original line or the orig line has caught up
               #   with the match line
 
               last if ($match_file->id() == $file_id &&
                        (($cur_match_line >= $line &&
                        $match_line < $line) ||
                        ($cur_orig_line >= $match_line &&
                          $line < $match_line)));
               $matches = 0;
               if ($cur_orig_fp && $cur_match_fp) {
                  if ($cur_orig_fp eq $cur_match_fp) {
                     $matches = 1;
                     $m_count++;
                     %done_line_hash->{$match_file->id()}
                          {$cur_orig_line,$cur_match_line} = 1;
                     if ($match_file->id() == $file_id) {
                       %done_line_hash->{$match_file->id()}
                          {$cur_match_line,$cur_orig_line} = 1;
                     }
                  }
                  $cur_orig_line++;
                  $cur_match_line++;
               }
               else
               {
                  my $orig_is_blank = 0;
                  my $match_is_blank = 0;
                  if ($blankline_hash_ref->{$file_id}) {
                     $orig_is_blank = 
                          $blankline_hash_ref->{$file_id}{$cur_orig_line};
                  }
                  if ($blankline_hash_ref->{$match_file->id()}) {
                     $match_is_blank = 
                          $blankline_hash_ref->{$match_file->id()}{$cur_match_line};
                  }
                  if ($orig_is_blank && $match_is_blank) {
                     $matches = 1;
                     $cur_orig_line++;
                     $cur_match_line++;
                  }
                  elsif ($orig_is_blank) {
                     $matches = 1;
                     $cur_orig_line++;
                  }
                  elsif ($match_is_blank) {
                     $matches = 1;
                     $cur_match_line++;
                  }
               }
               $last_line = $cur_orig_line;
               $last_match_line = $cur_match_line;
            }
            if ($m_count >= $argN)  {
               print "\n";
               print "FILE: " . $file->name() .  " LINE: $line\n";
               print "FILE: " . $match_file->name() . " LINE: $match_line\n";
               print "   Same for $m_count non-blank lines.\n";
            }
         }
      }
      %done_file_hash->{$file_id} = 1;
   }
   
}
















