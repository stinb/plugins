# Understand perl API program: deltametrics.pl 
#
# Synopsis: Outputs delta metrics between two code directories
#
# Categories: Comparison, Project Report
#
# Languages: Ada, Fortran, C
#
# Usage:
sub usage
{
    return << "END_USAGE";
${ \( shift @_ ) }
usage: deltametrics  -old dir|db -new dir|db -full 
 -old dir|db        (required) Old source dir|db
 -new dir|db        (required) New source dir|db
 -language          (optional) Define language (required if -old & -new are dirs)
						Possible language values:
							Ada
							C++
							Fortran
 -full              (optional) Lists all changed entities rather than just a count
 -help              (optional) Display help/usage message
END_USAGE
}

# Description:
#   Generate metrics report displaying the "delta metrics" for tracking the
#   progression of a project periodically, i.e. number of new lines
#   of code per file, number of new, modified or deleted functions.
#   
#   The old and new options can specify either database files or
#   source directory trees that will be analyzed for differences
#   in metrics between the two.  If directories are specified 
#   the command-line parser will be run to create temporary 
#   database files for analysis.
# 
#   For example, the report displays as,
#  
#   Changes in Directory foo:
#  
#   New file x, 100 lines, 5 classes, 9 functions....
#   New file y, ...
#   Deleted file z, ....
#   Modified file abc: 
#                      lines:        8 new, 7 modified, 6 deleted
#                      classes:      2 new, 2 modified, 1 deleted
#                      functions:    7 new, 3 modified, 0 deleted
#  
#
#  For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl/
#  Refer to the documenation for details on using the perl API 
#  with Understand and for details on all Understand perl API calls.
# 
#  22-Jun-2001 DLL
#
#  2 July  - change from $TMP to $TEMP and added validity checks
#          - add old and new directory validity checks
#          - remove "Declare" from ref kind string
#
#  3 July  - change ref kind string to "Declare" for ada and "Define" for C++
#          - 
#  25 Feb 2003 - change to allowing database to be specified rather 
#                than just source directories.   
#  26 Feb 2003 - add directory recursion, improved files filter and changed
#                to using @files form for undada and undc to prevent potential
#                command line limitation issues
#  18 April 2012 - KG - Update to use new version of und
#      
use strict;
use Understand;
use Getopt::Long;
use File::Basename;
use IO::File;
use POSIX qw(tmpnam);
use Env;
use File::Find;
use Cwd;


my $language;
my @old_files = ();
my @new_files = ();
my %old_projectMetrics = ();
my %new_projectMetrics = ();

my $oldOpt;
my $newOpt;
my $fullOpt;
my $langOpt;
my $helpOpt;
GetOptions(
	   "old=s" => \$oldOpt,
	   "new=s" => \$newOpt,
	   "language=s" => \$langOpt,
	   "full" => \$fullOpt,
	   "help" => \$helpOpt,
           );

# allow required arguments to be specified w/o options
foreach my $arg (@ARGV)
{
   unless ($oldOpt)  {$oldOpt = $arg; next;} 
   unless ($newOpt)  {$newOpt = $arg; next;} 
   unless ($langOpt)  {$langOpt = $arg; next;} 
}

# do help option
die usage("") if $helpOpt;

# insist the it not be run from within the GUI
if ( Understand::Gui::active() ) {
    die usage("This script is not designed to be called from the GUI");
}

# make sure required args are specified
die usage("Both -old and -new options required.") unless ($oldOpt && $newOpt);

my $oldDb;
my $newDb;
if ( -d $oldOpt ^ -d $newOpt ) {
    # only one is a directory
    die usage("Mis-match of files and directories in -old and -new args.");
}
elsif ( -d $oldOpt && -d $newOpt ) {
    # both are directories
    die usage("-language arg required for directories --> Valid options: Ada C Fortran") unless $langOpt;
    print "Creating Old Database                 ";
    $oldDb = build_database($oldOpt);
     print "\rCreating New Database              ";
    $newDb = build_database($newOpt);

}
else {
    # both are files (presumably a database file)
    die usage("Don't use -language arg for databases") if $langOpt;
    $oldDb = $oldOpt;
    $newDb = $newOpt;
}
# read in the database info
print "\rReading Old Database               ";
read_database($oldDb,\@old_files,\%old_projectMetrics);

print "\rReading New Database          ";
read_database($newDb,\@new_files,\%new_projectMetrics);
print "\r                                          ";
# report differences in project metrics
reportProjectDiffs($oldDb,$newDb);

# report differences in files
reportFileDiffs(\@old_files,\@new_files);

# subroutines 


sub getTmpFilename
{
    my $ext = shift;
    
    my $file;
    my $dir = getcwd;
    # get non-existant filename in that directory
    do { 
	$file = "$dir/deltametrics" . int(rand()*1000000) . $ext;
    } until ! -e "$file";
    return ($file);
}


sub findFiles
{
    my ($dir,$include) = @_;

    # recurse into this directory getting all files
    my @files;
    my %visited;
    
    find sub { 
	my $cwd = $File::Find::dir;
	my $file = $File::Find::name;

	if ( $cwd !~ /CVS$/ ) {
	    
	    # directories
	    if ( -d $_ ) {
		# report visiting this dir (only once)
		print "Scanning $cwd\n" if (!$visited{$cwd});
		$visited{$cwd} = 1;
	    }

	    # files
	    if ( -f $_ && $_ =~ /$include/ ) {
		# add matching file
		push (@files, $file);
	    }
	    
	}
    
    }, $dir;

    # sort alphabetically
    @files = sort(@files);

    return(@files);
}


sub build_database
{
    my ($dir) = @_;
    
    # get temporary database file
    my $database = getTmpFilename(".udb");

    # run appropriate understand parser
    my $und_cmd = "und -quiet -db $database create -languages $langOpt add $dir analyze";
    system("$und_cmd") && die "\ncannot execute understand command";

    # return 
    return($database);
}


sub reportProjectDiffs
{
    my ($old_db_name,$new_db_name) = @_;

    my $no_new_metrics = scalar(%new_projectMetrics);
    my $no_old_metrics = scalar(%old_projectMetrics);
    if ( scalar(%old_projectMetrics) != scalar(%new_projectMetrics) ) {
	die "different number of metrics";
    }

    print "\n";
    printStat("Project Metrics","old","new");
    printStat("----------------------","-----","-----");
    foreach my $metric (sort keys %new_projectMetrics) {
	printStat("$metric",$old_projectMetrics{$metric},$new_projectMetrics{$metric});
    }
}

sub readProjectMetrics
{
    my ($projectMetrics,$db) = @_;

    my @metricslist = $db->metrics();
    my @metrics = $db->metric(@metricslist);
    for (my $i=0; $i<scalar(@metrics); $i++) {
	$$projectMetrics{$metricslist[$i]} = $metrics[$i];
    }
}





sub reportFileDiffs
{
    my ($old_files,$new_files) = @_;

    my $old_idx;
    my $new_idx;

    while ( 1 ) {
	my $old = @$old_files[$old_idx];
	my $new = @$new_files[$new_idx];

	if ( !$old || !$new ) {
	    last;
	}

	my $old_name;
	my $new_name;
	my $old_fingerprint;
	my $new_fingerprint;

	if ( $old ) {
	    my ($db_file,$fileInfo,$fileMetrics) = @$old;
	    (my $longname,$old_name,my $age,$old_fingerprint,my $old_ents) = @$fileInfo;
	}
	if ( $new ) {
	    my ($db_file,$fileInfo,$fileMetrics) = @$new;
	    (my $longname,$new_name,my $age,$new_fingerprint,my $new_ents) = @$fileInfo;
	}

	if ( !$old_name ) {
	    printFile("New", $new);
	    $new_idx++;
	}
	elsif ( !$new_name ) {
	    printFile("Deleted", $old);
	    $old_idx++;
	}
	elsif ( ($old_name cmp $new_name) == 1 ) {
	    printFile("New", $new);
	    $new_idx++;
	}
	elsif ( ($old_name cmp $new_name) == 0 ) {
	    # name match
	    if ($old_fingerprint ne $new_fingerprint) {
		printFileDiffs($old,$new);
	    }
	    $old_idx++;
	    $new_idx++;
	}
	elsif ( ($old_name cmp $new_name) == -1 ) {
	    printFile("Deleted", $old);
	    $old_idx++;
	}
    }
}

sub printFileDiffs
{
    my ($old,$new) = @_;
    
    my ($old_db_file,$old_fileInfo,$old_fileMetrics) = @$old;
    my ($new_db_file,$new_fileInfo,$new_fileMetrics) = @$new;

    my ($old_longname,$old_name,$old_age,$old_fingerprint,$old_ents) = @$old_fileInfo;
    my ($new_longname,$new_name,$new_age,$new_fingerprint,$new_ents) = @$new_fileInfo;

    print "\n";
    printFileInfo("Modified",$old,$new);

    foreach my $ent (sort keys %$old_ents) {
	compareEnts($ent,$$old_ents{$ent},$$new_ents{$ent});
    }
}

sub printFileInfo
{
    my ($msg,$old_file,$new_file) = @_;

    my ($old_db_file,$old_fileInfo,$old_fileMetrics) = @$old_file if $old_file;
    my ($old_longname,$old_name,$old_age,$old_fingerprint,$old_ents) = @$old_fileInfo if $old_fileInfo;
    my ($old_CountLine,$old_CountLineCode,$old_CountLineComment,$old_CountStmtDecl,$old_CountStmtExe) = @$old_fileMetrics if $old_fileMetrics;

    my ($new_db_file,$new_fileInfo,$new_fileMetrics) = @$new_file if $new_file;
    my ($new_longname,$new_name,$new_age,$new_fingerprint,$new_ents) = @$new_fileInfo if $new_fileInfo;
    my ($new_CountLine,$new_CountLineCode,$new_CountLineComment,$new_CountStmtDecl,$new_CountStmtExe) = @$new_fileMetrics if $new_fileMetrics;

    my $filename = $new_name;
    $filename = $old_name unless $filename;

    printStat("$msg: $filename","old","new");
    printStat("----------------------","-----","-----");
    printStat("CountLine",$old_CountLine,$new_CountLine);
    printStat("CountLineCode",$old_CountLineCode,$new_CountLineCode);
    printStat("CountLineComment",$old_CountLineComment,$new_CountLineComment);
    printStat("CountStmtDecl",$old_CountStmtDecl,$new_CountStmtDecl);
    printStat("CountStmtExe",$old_CountStmtExe,$new_CountStmtExe);

}

sub printStat
{
    my ($stat,$old,$new) = @_;
    
    return if ( !$old && !$new );

    $old = "" if !$old;
    $new = "" if !$new;
    printf ("%-32s%-10s%-10s\n",$stat,$old,$new);
}



sub compareEnts
{
    my ($kind,$old_entities,$new_entities) = @_;

    #print "kind=$kind\n";
    #print scalar(@$old_entities) . " old, " . scalar(@$new_entities) . " new entries\n";

    printStat("Count".$kind."s",scalar(@$old_entities),scalar(@$new_entities));
    
    my $old_idx;
    my $new_idx;

    my @new_ents = ();
    my @modified_ents = ();
    my @deleted_ents = ();
    
    # loop through and compare entities in this list
    while ( 1 ) {
	my $old = @$old_entities[$old_idx];
	my $new = @$new_entities[$new_idx];

	if ( !$old || !$new ) {
	    last;
	}

	my $new_ent=0;
	my $modified_ent=0;
	my $deleted_ent=0;

	my ($old_longname,$old_kindname,$old_line,$old_column,$old_end_line,
	    $old_end_column,$old_fingerprint,$old_uniquename,$old_entMetrics) = @$old if $old;
	my ($new_longname,$new_kindname,$new_line,$new_column,$new_end_line,
	    $new_end_column,$new_fingerprint,$new_uniquename,$new_entMetrics) = @$new if $new;

	if ( !$old_uniquename ) {
	  # print "New ent: ";
	  # print_ent( $new );
	    $new_ent = 1;
	    $new_idx++;
	}
	elsif ( !$new_uniquename ) {
	  # print "Deleted ent: ";
	  # print_ent( $old );
	    $deleted_ent = 1;
	    $old_idx++;
	}
	elsif ( ($old_uniquename cmp $new_uniquename) == 1 ) {
	  # print "New ent: ";
	  # print_ent( $new );
	    $new_ent = 1;
	    $new_idx++;
	}
	elsif ( ($old_uniquename cmp $new_uniquename) == 0 ) {
	    # comparision 
	    if ( $old_fingerprint != $new_fingerprint ) {
		$modified_ent=1;
	#print "Modified ent: ";
	#print_ent( $new );
	    }
	    $old_idx++;
	    $new_idx++;
	}
	elsif ( ($old_uniquename cmp $new_uniquename) == -1 ) {
	#   print "Deleted ent: ";
	#   print_ent( $old );
	#    print_ent( $old ); print_ent( $new) ;
	    $deleted_ent = 1;
	    $old_idx++;
	}

	if ( $new_ent ) {
	    push( @new_ents, [ $new_longname,
			       $new_kindname,
			       $new_line, 
			       $new_column,
			       $new_end_line,
			       $new_end_column,
			       $new_fingerprint,
			       $new_uniquename,
			       $new_entMetrics,
			       ] );
	    print "New: $new_kindname: $new_longname, " , 
	    "$new_line/$new_column thru $new_end_line/$new_end_column\n" if $fullOpt;
	}
	if ( $modified_ent ) {
	    push( @modified_ents, [ $new_longname,
				    $new_kindname,
				    $new_line, 
				    $new_column,
				    $new_end_line,
				    $new_end_column,
				    $new_fingerprint,
				    $new_uniquename,
				    $new_entMetrics,
				    ] );
	    print "Modified: $new_kindname: $new_longname, \n" , 
	    "       $old_line/$old_column thru $old_end_line/$old_end_column, $old_fingerprint\n" ,
	    "       $new_line/$new_column thru $new_end_line/$new_end_column, $new_fingerprint\n" if $fullOpt;
	}
	if ($deleted_ent) {
	    push( @deleted_ents, [ $old_longname,
				   $old_kindname,
				   $old_line, 
				   $old_column,
				   $old_end_line,
				   $old_end_column,
				   $old_fingerprint,
				   $old_uniquename,
				   $old_entMetrics,
				   ] );

	    print "Deleted: $old_kindname: $old_longname, " , 
	    "$old_line/$old_column thru $old_end_line/$old_end_column\n" if $fullOpt;
	}
    }

    printEntMods($kind,\@new_ents,\@modified_ents,\@deleted_ents);
}

sub printEntMods
{
    my ($kind,$new,$modified,$deleted) = @_;

    my $CountNew = scalar(@$new) if $new;
    my $CountModified = scalar(@$modified) if $modified;
    my $CountDeleted = scalar(@$deleted) if $deleted;

    printStat("CountNew$kind","",$CountNew);
    printStat("CountModified$kind","",$CountModified);
    printStat("CountDeleted$kind","",$CountDeleted);

}




sub printFile
{
    my ($msg,$file) = @_;

    print "\n";
    if ( $msg =~ "New" ) {
	printFileInfo($msg,undef,$file);
    }
    else {
	printFileInfo($msg,$file,undef);
    }

}



sub openDatabase
{
    my ($file) = @_;    
    
    # open the database
    my ($db, $status) = Understand::open($file);
    die "Failure to open $file: ",$status,"\n" if $status;
    return ( $db );
}

sub closeDatabase
{
    my ($db) = @_;
    die "\$db is null, no database\n" if (!$db); 
    $db->close();
}



sub read_database
{
    my ($db_file,$files_array_ref,$projectMetrics_hash_ref) = @_;    

    my $db = openDatabase($db_file);

    # make sure this database is the same language as previous
    if ( $language && $language != $db->language() ) {
	print "    Language mismatch\n";  
	exit;
    }
    $language = $db->language();
    
    # get sorted file entities list
    my $kind = "File ~unknown ~unresolved";
    foreach my $ent (sort{$a->longname() cmp $b->longname();} $db->ents($kind)) {

	my $filename = $ent->longname();

	# print the filename
	#print "Getting entities for: $filename\n";

	# create lexer object
	my ($lexer,$status) = $ent->lexer();
	if ($status) {
	    print "file $status, cannot examine data\n";
	    return;
	}

	# open file directly and calculate a finger print
	# hopefully save time later if no changes
	open(FILE,"< $filename");
	my $string;
	while (<FILE>) {
	    $string = $string . $_;
	} 
	close(FILE);
	my $fingerprint = Understand::Util::checksum($string);

	my %ents = ();

	$ents{"Function"} = get_ents_by_file($db,$ent,"Function",$lexer);
	$ents{"Class"} = get_ents_by_file($db,$ent,"Class",$lexer);
	$ents{"Struct"} = get_ents_by_file($db,$ent,"Struct",$lexer);
	$ents{"Procedure"} = get_ents_by_file($db,$ent,"Procedure",$lexer);
	$ents{"Package"} = get_ents_by_file($db,$ent,"Package",$lexer);
	$ents{"Macro"} = get_ents_by_file($db,$ent,"Macro",$lexer);

	# print_ents($functions);
	
	my $file_age = -M $ent->longname();

	my @fileMetrics = [
			   $ent->metric("CountLine"),
			   $ent->metric("CountLineCode"),
			   $ent->metric("CountLineComment"),
			   $ent->metric("CountStmtDecl"),
			   $ent->metric("CountStmtExe"),
			   ];
	
	my @fileInfo=[
		      $ent->longname(),
		      $ent->name(),
		      $file_age,
		      $fingerprint,
		      \%ents,
		      ];
			
	push( @$files_array_ref,[$db_file,@fileInfo,@fileMetrics]);
    }
    
    readProjectMetrics($projectMetrics_hash_ref,$db);

    # close and remove database
    closeDatabase($db);

    # remove temporary database files if directories were given
    if ( -d $oldOpt && -d $newOpt ) {
	unlink($db_file) or die "Couldn't remove temporary database $db_file : $!";
    }
}



sub get_ents_by_file
{
  my ($db,$file_ent,$entkind,$lexer) = @_;

  # print "Getting $entkind entities\n";
  
  my @ents = ();

  my $refkind;
  my $lang = $db->language();
  if ( $lang =~ /Ada83/i || $lang =~ /Ada95/i) {
      $refkind = "Declare";
  }
  elsif ( $lang =~ /C/i ) {
      $refkind = "Define";
  }
  elsif ( $lang =~ /F/i ) {
      $refkind = "Define";
  }

  my @refs = $file_ent->refs($refkind,$entkind);
  my @sorted_refs = sort{$a->ent()->longname() cmp $b->ent()->longname();} @refs;
  foreach my $ref (@sorted_refs) {


      my ($fingerprint,$end_line,$end_column) = get_fingerprint($lexer,$ref);

      my $uniquename = $file_ent->name() . "|" . $ref->ent()->name() . 
	      "|" . $ref->ent()->kindname();

      my $entMetrics = [
			$ref->ent()->metric("CountLine"),
			$ref->ent()->metric("CountLineCode"),
			$ref->ent()->metric("CountLineComment"),
			$ref->ent()->metric("CountStmtDecl"),
			$ref->ent()->metric("CountStmtExe"),
			];

      push( @ents, [ 
		     $ref->ent()->longname(),
		     $ref->ent()->kindname(),
		     $ref->line(), 
		     $ref->column(),
		     $end_line,
		     $end_column,
		     $fingerprint,
		     $uniquename,
		     $entMetrics,
		     ] );
      
      #print_ent( @ents[scalar(@ents)-1] );
      
  }

  #print_ents(\@ents);
  return \@ents;
}

sub print_ents()
{
    my ($ents) = @_;

    foreach my $ent (@$ents) {
	print_ent($ent);
    }
}

sub print_ent()
{
    my ($info) = @_;

    my ($longname,$kindname,$line,$column,$end_line,
	$end_column,$fingerprint,$uniquename,$entMetrics) = @$info;

    my ($CountLine,$CountLineCode,$CountLineComment,$CountStmtDecl,$CountStmtExe) = @$entMetrics;
    
    print "  " . $longname . ", " . $kindname; 
    print ", " . $line . "/" . $column;
    print " thru " . $end_line . "/" . $end_column;
    print ", " . $fingerprint;
    print ", " . $uniquename;
    print "\n";
}

sub get_fingerprint()
{
    my ($lexer,$ref) = @_;

    my ($end_ref) = $ref->ent()->refs("End","",1);
    if ( $end_ref) {
	
	my $string;
	my $lexeme = $lexer->lexeme($ref->line(),$ref->column());

	while($lexeme) { 
	    
	    # if not a comment then add it 
	    if ( $lexeme->token() !~ "Comment" ) {
		$string = $string . $lexeme->text();
	    }
	    
	    # break when it's line/column numbers match the last one
	    if ( $lexeme->line_end =~ $end_ref->line() 
		 && $lexeme->column_end =~ $end_ref->column() ) {
		last;
	    }
	    
	    # increment to next one
	    $lexeme=$lexeme->next;
	}
	
	return(Understand::Util::checksum($string),$end_ref->line(),$end_ref->column());
    }
    else {
	return(undef,undef,undef);
    }
}



