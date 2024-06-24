# a_duplicate_stmts.pl
#
# Synopsis: Report duplicate statements
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
usage: ada_duplicate_lines  -db database\n
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
use Tie::RefHash;

my $argHelp;
my $argDb;
my $argN = 5;
my $db;
my %checksum_hash;
my %nextstmt_hash;

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
   # calculate checksums for statements in current file
   calcStmtChecksums($f_ent, \%checksum_hash, \%nextstmt_hash);
}

printDuplicateInfo(\%checksum_hash, \%nextstmt_hash);

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

sub calcStmtChecksums
{
   my ($file, $checksum_hash_ref, $nextstmt_hash_ref) = @_; 
   # checksum_hash_ref is ref to hash of checksum for arrays of 
   #  file/line records
   # nextstmt_hash_ref is ref to hash of hash {filename}{stmtline,stmtcol} ->
   #     nextstmtline,stmtcol 
   my %lexeme;
   my @stmt_starts = ();

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
      my $line;
      my $column;

      ($fingerprint, $lexeme, $line, $column) = 
                           getStmtFingerprint($lexeme); 
#print "line: $line, column: $column\n";
      if ($fingerprint) {
         # add to hash checksum->file/line array
        my $rec = {FILE => $file, LINE => $line, COLUMN => $column};
        my $loc_rec = {LINE => $line, COLUMN => $column};
        if (!{$checksum_hash_ref->{$fingerprint}}) {
            @{$checksum_hash_ref->{$fingerprint}} = ();
         }
         push @{$checksum_hash_ref->{$fingerprint}}, $rec;
         push @stmt_starts, $loc_rec;
      }
   }
   # make hash so we can look up next line that starts a new statement
   my $prev_strt_line;
   my $prev_strt_col;
   $nextstmt_hash_ref->{$file->id()} = {};
   foreach my $strt_loc (@stmt_starts) {
      my $strt_line = $strt_loc->{LINE};
      my $strt_col = $strt_loc->{COLUMN};
      my %new_loc_rec;
      %new_loc_rec->{LINE} = $strt_line;
      %new_loc_rec->{COLUMN} = $strt_col;
      if ($prev_strt_line) {
         if (!$nextstmt_hash_ref->{$file->id()}) {
            $nextstmt_hash_ref->{$file->id()} = {};
         }
         $nextstmt_hash_ref->{$file->id()}{$prev_strt_line,
                              $prev_strt_col} = {};
         $nextstmt_hash_ref->{$file->id()}{$prev_strt_line,
                              $prev_strt_col} {LINE} = $strt_loc->{LINE};
         $nextstmt_hash_ref->{$file->id()}{$prev_strt_line,
                              $prev_strt_col} {COLUMN} = $strt_loc->{COLUMN};
      }
      $prev_strt_line = $strt_line;
      $prev_strt_col = $strt_col;
   }
# debug
#  foreach my $file_id (sort keys %$nextstmt_hash_ref) {
#     my $cur_file = $db->ent_from_id($file_id);
#     my %stmt_hash = %{$nextstmt_hash_ref->{$file_id}};
#     print "File: " . $cur_file->name() . "\n";
#     foreach my $cur_strt_key (keys %stmt_hash) {
#        my $next_line;
#        my $next_col;
#        print "key: " .  $cur_strt_key . "\n";
#        my ($cur_line, $cur_col) = split /$;/, $cur_strt_key;
#        print "line :"
#                          .
#                          $nextstmt_hash_ref->{$file_id}{$cur_strt_key}{LINE}
#                          . "\n";
#        print "column :"
#                          .
#                          $nextstmt_hash_ref->{$file_id}{$cur_strt_key}{COLUMN}
#                          . "\n";
#     }
#  }

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



sub getStmtFingerprint
{
   my ($lexeme) = @_;
   my $string; 
   my $in_whitespace = 0;
   my $line = 0;
   my $column = 0;
   my $pu_start = 0;
   my $single_key_stmt = 0;
   my $end_stmt = 0;
   my $paren_cnt = 0;
   # skip over leading white space
   while ($lexeme && ($lexeme->token() =~ "Whitespace" ||
                      $lexeme->token() =~ "Comment" ||
                      $lexeme->token() =~ "Newline")) {
      $lexeme = $lexeme->next;
   }
   if ($lexeme) {
      if ($lexeme->token() =~ "Keyword") {
         my $start_tok_text = $lexeme->text();
         $start_tok_text = "\L$start_tok_text\E";
         if ($start_tok_text eq "package" ||
             $start_tok_text eq "procedure" ||
             $start_tok_text eq "function" ||
             $start_tok_text eq "task" ||
             $start_tok_text eq "entry" ||
             $start_tok_text eq "protected") {
            $pu_start = 1;
         }
         elsif ($start_tok_text eq "begin") {
            $single_key_stmt = 1;
         }
      }
   }
   if ($lexeme) {
      $line = $lexeme->line_begin();
      $column = $lexeme->column_begin();
   }
   while ($lexeme && !$end_stmt) {
      # for comments/whitespace, add a single blank
      if ($lexeme->token() =~ "Whitespace" || $lexeme->token() =~ "Newline") {
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
      if ($pu_start) {
         if ($lexeme->text() eq "(") {
            $paren_cnt++;
         }
         elsif ($lexeme->text() eq ")") {
            $paren_cnt--;
         }
         elsif ($lexeme->text() eq ";" &&
                $paren_cnt < 1) {
            $end_stmt = 1;
         }
         elsif ($lexeme->text() =~ /is/i) {
            $end_stmt = 1;
         }
      }
      elsif ($single_key_stmt) {
         $end_stmt = 1;
      }
      elsif ($lexeme->text eq ";") {
         $end_stmt = 1;
      }
      $lexeme = $lexeme->next;
   }
   # advance past stmt end
   if ($lexeme) {
      $lexeme = $lexeme->next;
   }
   # strip trailing blank
   $string =~ s/ $//;
   # force string to lower case
   $string = "\L$string\E";
   if ($string && $string ne " ") {
      return(Understand::Util::checksum($string), $lexeme, $line, $column);
   }
   else {
      return(undef, $lexeme, 0, 0);
   }
}

sub numerically {$a <=> $b}

sub printDuplicateInfo
{
   my ($checksum_hash_ref, $nextstmt_hash_ref) = @_;
   tie my %dup_fileline_hash, 'Tie::RefHash';
      # hash of file to hash of line to checksum
   my %done_file_hash = {};

   foreach my $fp (keys %$checksum_hash_ref) {
      my @file_line_array = @{$checksum_hash_ref->{$fp}};
      # if there were duplicates for the current checksum
      if ($#file_line_array > 0) {
         foreach my $rec (@file_line_array) {
            my $r_file = $rec->{FILE};
            my $r_line = $rec->{LINE};
            my $r_col  = $rec->{COLUMN};
            if (!$dup_fileline_hash{$r_file}) {
               $dup_fileline_hash{$r_file} = {};
            }
            $dup_fileline_hash{$r_file}{$r_line}{$r_col} = $fp;
         }
      }
   }
   foreach my $file (keys %dup_fileline_hash) {
	   

	  tie my %done_line_hash, 'Tie::RefHash';
      foreach my $line (sort numerically 
           keys %{$dup_fileline_hash{$file}}) {
         foreach my $col (sort numerically
           keys %{$dup_fileline_hash{$file}{$line}}) { 
            my $fp = $dup_fileline_hash{$file}{$line}{$col};
            my @file_line_array = @{$checksum_hash_ref->{$fp}};
            foreach my $rec (@file_line_array) {
               my $match_file = $rec->{FILE};
               my $match_line = $rec->{LINE};
               my $match_col = $rec->{COLUMN};
               my $cur_orig_line = 
                   $nextstmt_hash_ref->{$file}{$line,$col}{LINE};
               my $cur_orig_col = 
                   $nextstmt_hash_ref->{$file}{$line,$col}{COLUMN};
               my $cur_match_line = 
                  $nextstmt_hash_ref->{$match_file}
                                       {$match_line,$match_col}{LINE};
               my $cur_match_col = 
                  $nextstmt_hash_ref->{$match_file}
                                       {$match_line,$match_col}{COLUMN};
               my $m_count = 1;
               my $matches = 1;
               my $last_line = $line;
               my $last_col = $col;
               my $last_match_line = $match_line;
               my $last_match_col = $match_col;
               my $final_orig_line = $line;
               my $final_match_line = $match_line;
               # skip record if this is the current file/line
               next if ($match_line == $line && $match_col == $col &&
                        $match_file == $file);
               # skip record if the file has already been done
               next if (%done_file_hash->{$match_file});
               # skip record if this file/line has already been reported
               #   as part of another sequence
               if (!%done_line_hash->{$match_file}) {
                  %done_line_hash->{$match_file} = {};
               }
               next if (%done_line_hash->{$match_file}{$line,$match_line});
               %done_line_hash->{$match_file}
                    {$line,$match_line} = 1;
               if ($match_file == $file) {
                 %done_line_hash->{$match_file}
                    {$match_line,$line} = 1;
               }
               while ($matches) {
                  my $cur_orig_fp = %dup_fileline_hash->{$file}
                                       {$cur_orig_line}{$cur_orig_col};
                  my $cur_match_fp = %dup_fileline_hash->{$match_file}
                                        {$cur_match_line}{$cur_match_col};
                  # stop if both code segments are in the same file and
                  #   the current match line has caught up with the
                  #   original line or the orig line has caught up
                  #   with the match line
                  last if ($match_file == $file &&
                           (($cur_match_line >= $line &&
                           $match_line < $line) ||
                           ($cur_orig_line >= $match_line &&
                             $line < $match_line)));
                  $matches = 0;
                  if ($cur_orig_fp && $cur_match_fp) {
                     if ($cur_orig_fp eq $cur_match_fp) {
                        $matches = 1;
                        $m_count++;
                        $final_orig_line = $cur_orig_line;
                        $final_match_line = $cur_match_line;
                        %done_line_hash->{$match_file}
                             {$cur_orig_line,$cur_match_line} = 1;
                        if ($match_file == $file) {
                          %done_line_hash->{$match_file}
                             {$cur_match_line,$cur_orig_line} = 1;
                        }
                     }
                     my $save_orig_line = $cur_orig_line;
                     $cur_orig_line =
                        $nextstmt_hash_ref->{$file}
                            {$cur_orig_line,$cur_orig_col}{LINE};
                     $cur_orig_col =
                        $nextstmt_hash_ref->{$file}
                            {$save_orig_line,$cur_orig_col}{COLUMN};
                     my $save_match_line = $cur_match_line;
                     $cur_match_line = 
                        $nextstmt_hash_ref->{$match_file}
                            {$cur_match_line,$cur_match_col}{LINE};
                     $cur_match_col = 
                        $nextstmt_hash_ref->{$match_file}
                            {$save_match_line,$cur_match_col}{COLUMN};
                  }
                  $last_line = $cur_orig_line;
                  $last_col = $cur_orig_col;
                  $last_match_line = $cur_match_line;
                  $last_match_col = $cur_match_col;
               }
               if ($m_count >= $argN)  {
                  print "\n";
                  print "FILE: " . $file->name() .  " LINES: $line" 
                                   . " through $final_orig_line\n";
                  print "FILE: " . $match_file->name() . " LINES: $match_line" 
                                   . " through $final_match_line\n";
                  print "   Same for $m_count statements.\n";
               }
            }
         }
      }
      %done_file_hash->{$file} = 1;
   }
   
}
















