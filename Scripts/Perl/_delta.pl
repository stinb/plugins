#
# Synopsis: Summarize statement differences between project versions
#
# Categories: Comparison, Project Report
#
# Languages: Ada, Fortran, C, Java
#
# TBD 
#
#  For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl/
#  Refer to the documenation for details on using the perl API 
#  with Understand and for details on all Understand perl API calls.
# 
use strict;
use Understand;
use Getopt::Long;
use File::Basename;
use IO::File;
use Env;
use File::Find;
use Algorithm::Diff qw(traverse_sequences diff);

autoflush STDOUT 1;

sub usage
{
    return << "END_USAGE";
${ \( shift @_ ) }
usage: _delta.pl -db db [-prev db] options...
 -db db           Understand database (required for uperl, inherited from Understand)
 -comp type       (optional) Comparison type - lines, statements, loc, comments, functions
 -prev db         (optional) Previous db
 -out file        (optional) Output file, default is stdout
 -diffs           (optional) Show differences
 -perfile         (optional) Show per-file summaries
 -perarch         (optional) Show per-architecture summaries
 -files           (optional) Show filenames as they are read 
 -help            (optional) Display help/usage message
END_USAGE
}


my %opts=( 
    db => '',
    comp => 'lines',
    prev => '',
    out => '',
    diffs => '',
    perfile => '',
    chunks => '',
    files => '',
    help => '',
    );

GetOptions(
	   "db=s" => \$opts{db},
	   "comp=s" => \$opts{comp},
	   "prev=s" => \$opts{prev},
	   "out=s" => \$opts{out},
	   "diffs" => \$opts{diffs},
	   "perfile" => \$opts{perfile},
     "perarch" => \$opts{perarch},
	   "chunks" => \$opts{chunks},
	   "files" => \$opts{files},
	   "help" => \$opts{help},
           );

# do help option
die usage("") if ($opts{help});

if (! $opts{db}){
  if( ! Understand::Gui::active() && ! Understand::Gui::db()){
    die usage("database specification required");
  }else{
    $opts{db} = Understand::Gui::db->name;
  }
}


# make sure db files are readable before continuing on
die "Can't read database " . $opts{'db'} if ( $opts{db} && ! -r $opts{db} );
die "Can't read database " . $opts{'prev'} if ( $opts{prev} && ! -r $opts{prev} );


my $outFh;
if ( $opts{out} ) {
    open($outFh, "> $opts{out}") or die "Can't open file $opts{out}: $!\n";
    print STDOUT "Writing output to ", $opts{out}, "\n";
    select($outFh);
}

my %archs;
my $files = {};
my $summaries = {
    filesAdded => 0,
    filesChanged => 0,
    filesCurrent => 0,
    filesDeleted => 0,
    statementsAdded => 0,
    statementsChanged => 0,
    statementsCurrent => 0,
    statementsDeleted => 0,
};

# read in "curr" database info
($summaries->{filesCurrent},$summaries->{statementsCurrent}) = readDb($opts{db});


# only one db defined, just print stats for it and exit
if (!$opts{prev}) {
  printProjectFileSummary($opts{db}) if ($opts{perfile}) ;
  printArchitectureSummary($opts{db}) if ($opts{perarch}) ;
  printOneProjectSummary();
}else{

  # read in "prev" database info, it tests for matching $language
  readDb($opts{prev});

  # compare the two files lists 
  diffFilesList($opts{prev},$opts{db});

  # compare corresponding files
  diffCorrespondingFiles();

  # print file differences summary
  printFileDiffsSummary() if ($opts{perfile});
  
  # print architecture differences summary
  printArchDiffsSummary(($opts{prev},$opts{db})) if ($opts{perarch});

  # show the file differences
  printFileDiffs() if ($opts{diffs});

  # print out the project summary
  printTwoProjectSummary();
}


# subroutines

sub printTwoProjectSummary
{
	my $filesPrevious = $summaries->{filesCurrent} +
						$summaries->{filesDeleted} -
						$summaries->{filesAdded};

    print "\nSummary:\n";
    print "  ", $summaries->{filesAdded}, " files added\n";
    print "  ", $summaries->{filesDeleted}, " files deleted\n";
    print "  ", $summaries->{filesChanged}, " files changed\n";
    print "  ", $summaries->{filesCurrent}, " files in new version\n";
    print "  ", $filesPrevious, " files in old version\n";
    print "  ", $summaries->{statementsAdded}, " ", $opts{comp} , " added, ",
                $summaries->{statementsDeleted}, " deleted, ",
                $summaries->{statementsChanged}, " changed. ",
                "Total ", $opts{comp}, " ", $summaries->{statementsCurrent}, " in current project\n";
}


sub diffCorrespondingFiles
{
    my $correspond = $files->{'correspond'};

    # calculate differences 
    my $qty = scalar(@{$correspond});
    print STDOUT "Comparing $qty corresponding files\n";
    foreach my $c (@{$correspond}) {

	print STDOUT "."; #Print a dot for each file looked at.
	my $fileHashP = $c->{prev};
	my $fileHashC = $c->{curr};
	my $statementsP = $fileHashP->statements();
	my $statementsC = $fileHashC->statements();

        if (@$statementsP > 10000){
            print $c->{prev}->relname." is too large to compare, skipping...\n";
            next;
        }

	# expand into array[2] of file hashes
	my $diffInfo = diffStatements($statementsP,$statementsC);
	my $added = $diffInfo->{added};
	my $changed = $diffInfo->{changed};
	my $deleted = $diffInfo->{deleted};
	my $text = $diffInfo->{text};

	# update summaries with stats for this file
	$summaries->{filesChanged}++ if ( $added || $deleted || $changed );
	$summaries->{statementsAdded} += $added;
	$summaries->{statementsChanged} += $changed;
	$summaries->{statementsDeleted} += $deleted;
	
	# save diffInfo in current file's hash
	$fileHashC->diffs($diffInfo);

    }
    print STDOUT "\n";
}

sub printFileDiffsSummary
{

    my $correspond = $files->{'correspond'};

    #print "\nSummary of File Comparision\n\n";

    # show differences summary
	foreach my $c (@{$correspond}) {
		my $file = $c->{curr};
		my $label = $opts{comp};
		my $diffInfo = $file->diffs();
		my $added = $diffInfo->{added};
		my $changed = $diffInfo->{changed};
		my $deleted = $diffInfo->{deleted};
		my $stmts = $file->statementCount();
		my $filename = $file->relname();
		
		printf("%-20s $label: $stmts", $filename . ":");
		if ( $added || $deleted || $changed ) {
			print ", added: $added, deleted: $deleted, changed: $changed";
		}
		print "\n";
	}
}

sub printArchDiffsSummary
{
  print "\nSummary of Architecture Comparision\n\n";
  
  foreach my $c (@{$files->{'added'}}){
    my $fileArchs = $c->archs();
    foreach my $arch (keys %$fileArchs){
      $archs{$arch}{stmts}   += $c->statementCount();
      $archs{$arch}{added} += $c->statementCount();
    }
  }
  
  foreach my $c (@{$files->{'deleted'}}){
    my $fileArchs = $c->archs();
    foreach my $arch (keys %$fileArchs){
      $archs{$arch}{deleted} += $c->statementCount();
    }
  }
  
  #This handles files that exist in both projects
  my $label = $opts{comp};
  foreach my $c (@{$files->{'correspond'}}) {
    my $file = $c->{curr};
    my $diffInfo = $file->diffs();
    my $fileArchs = $file->archs();
    foreach my $arch (keys %$fileArchs){
      $archs{$arch}{stmts}   += $file->statementCount();
      $archs{$arch}{added}   += $diffInfo->{added};
      $archs{$arch}{deleted} += $diffInfo->{deleted};
      $archs{$arch}{changed} += $diffInfo->{changed};
    }
  }
  
  #Print Results
  foreach my $arch(keys %archs){
    printf("%-25s $label: ".$archs{$arch}{stmts}, $arch . ":");
    print ", added: $archs{$arch}{added}, deleted: $archs{$arch}{deleted}, changed: $archs{$arch}{changed}\n";
  }
  
  print "\n";
}



sub printFileDiffs
{
    my $correspond = $files->{'correspond'};

    # show differences 
    print "\nFile Differences\n";

    foreach my $c (@{$correspond}) {
	my $file = $c->{curr};
	my $label = $opts{comp};
	my $filename = $file->relname(); 
	my $diffInfo = $file->diffs();
	my $added = $diffInfo->{added};
	my $changed = $diffInfo->{changed};
	my $deleted = $diffInfo->{deleted};
	my $text = $diffInfo->text();
	if ( $added || $deleted || $changed ) {
	    print "Differences in $filename\n\n";
	    print $text;
	    print "--------\n\n";
	}
    }
}


my $File_Length_Difference = 0;
my $diffText;

sub diffStatements
{
    my ($statementsP, $statementsC) = @_;

    my ($added,$changed,$deleted);
    
    # diff yields lots of pieces, each of which is basically a Block object
    my $diffs = diff($statementsP,$statementsC,\&Statement::text);

    # count diffs and create diffsText
    $diffText = '';
    my $Context_Lines = 0; # lines of context to print. 0 for old-style diff
    my ($hunk,$oldhunk);
    # Loop over hunks. If a hunk overlaps with the last hunk, join them.
    # Otherwise, print out the old one.
    foreach my $piece (@$diffs) {
	$hunk = new Hunk ($piece, $Context_Lines);
	next unless $oldhunk; # first time through
	
	# Don't need to check for overlap if blocks have no context lines
	if ($Context_Lines && $hunk->does_overlap($oldhunk)) {
	    $hunk->prepend_hunk($oldhunk);
	} else {
	    my ($a,$c,$d) = $oldhunk->output_old_diff($statementsP, $statementsC);
	    $added += $a;
	    $changed += $c;
	    $deleted += $d;
	}
	
    } continue {
	$oldhunk = $hunk;
    }
    # possible last hunk
    my ($a,$c,$d) = $oldhunk->output_old_diff($statementsP, $statementsC) if ($oldhunk);
    $added += $a;
    $changed += $c;
    $deleted += $d;
    
    my $diffInfo = DiffInfo->new(
				 added   => $added,
				 changed => $changed,
				 deleted => $deleted,
				 text    => $diffText,
				 );
    return( $diffInfo );

}


sub diffFilesList
{
    my ($dbP_file,$dbC_file) = @_;

    # get lists from files hash
    my $listP = $files->{$dbP_file};
    my $listC = $files->{$dbC_file};
    my $countP = scalar(@$listP);
    my $countC = scalar(@$listC);

    # initialize arrays
    my $added = [];
    my $deleted = [];
    my $correspond = [];

    # build array of relnames for comparison
    print STDOUT "Comparing file lists (current: $countC, previous: $countP)\n";
    traverse_sequences $listP, $listC, { 
	DISCARD_A => sub {
	    print STDOUT "."; 
	    my $fileinfo = $listP->[shift];
	    push(@{$deleted},$fileinfo);
	    $summaries->{filesDeleted}++;
	    $summaries->{statementsDeleted} += $fileinfo->statementCount();
	},
	DISCARD_B => sub { 
	    print STDOUT "."; 
	    my $fileinfo = $listC->[shift,shift];
	    push(@{$added},$fileinfo);
	    $summaries->{filesAdded}++;
	    $summaries->{statementsAdded} += $fileinfo->statementCount();
	},
	MATCH     => sub {
	    print STDOUT "."; 
	    my $fileinfoP = $listP->[shift];
	    my $fileinfoC = $listC->[shift];
	    push(@{$correspond}, { prev => $fileinfoP, curr => $fileinfoC } );
	},
    }, \&FileInfo::relname;
    print STDOUT "\n";

    # print out
    if ($opts{perfile} ) {

	print "\nSummary of File Comparision\n\n";

	if (@{$added}) {
	    print scalar(@{$added}), " added files:\n\n";
	    foreach my $fileinfo (@{$added}) {
		my $relname = $fileinfo->relname();
		my $count = $fileinfo->statementCount();
                print $relname, ": ", $count, " ", $opts{comp}, "\n";
	    }
	    print "\n";
	}
	if (@{$deleted}) {
	    print scalar(@{$deleted}), " deleted files:\n\n";
	    foreach my $fileinfo (@{$deleted}) {
		my $relname = $fileinfo->relname();
		my $count = $fileinfo->statementCount();
                print $relname, ": ", $count, " ", $opts{comp}, "\n";
	    }
	    print "\n";
	}
    }

    $files->{'added'} = $added;
    $files->{'deleted'} = $deleted;
    $files->{'correspond'} = $correspond;
}

sub printProjectFileSummary
{
    my ($db_file) = @_;    
    print "Summary of files in $db_file\n";
    my $fileList = $files->{$db_file};

    foreach my $file (@{$fileList}) {
	my $relname = $file->relname();
	my $count = $file->statementCount();

	print $relname, ": ", $count, " ", $opts{comp}, "\n";

	# optionally show calculated metrics
	if ( $opts{metrics} ) {
	    my $metrics = $file->metrics();
	    if ( scalar(%$metrics) ) {
		print "  Metrics:\n";
		foreach my $key ( sort (keys(%$metrics) ) ) {
		    print "    $key: ", $file->metrics()->{$key}, "\n";
		}
		print "\n";
	    }
	}

    }
}

sub printArchitectureSummary
{
    my ($db_file) = @_;    
    print "\nSummary of Architectures in $db_file\n";
    
    my $fileList = $files->{$db_file};
    
    foreach my $file (@{$fileList}) {
      my $fileArchs = $file->archs();
      foreach my $arch (keys %$fileArchs){
         $archs{$arch}{count} += $file->statementCount();
         $archs{$arch}{file} ++;
      }
    }
    
    foreach my $arch(keys %archs){
      print $arch. ": ". $archs{$arch}{count} ." ".$opts{comp}.", ".$archs{$arch}{file}." files\n";
    }
}


sub printOneProjectSummary
{
    print "\nSummary:\n";
    print $summaries->{filesCurrent}, " files in project\n";
    print $summaries->{statementsCurrent}, " total ", $opts{comp}, "\n";
}

sub readDb
{
    my ($db_file) = @_;    
    my $isGUI;
    my $db;
    
    $isGUI = 1 if(Understand::Gui::active() && Understand::Gui::db && Understand::Gui::db->name eq $db_file);
    
    if ($isGUI){
      $db = Understand::Gui::db();
      die "Could not read database from Understand\n" unless $db;
    }else{
      ($db, my $status) = Understand::open($db_file);
      die "Failure to open $db_file: ",$status,"\n" if $status;
    }
    

    print STDOUT "Reading Understand database, $db_file\n";

    # make sure this database is the same language as previous
    die("language mismatch\n") if ( $files->{'language'} && $files->{'language'} != $db->language() );
    $files->{'language'} = $db->language();

    # get sorted file entities list
    my $kind = "File ~unknown ~unresolved";
    my @sorted = sort {$a->relname() cmp $b->relname();} 
                 (grep {$_->library() !~ /standard|iee|std/i} $db->ents($kind));
    

    
    my $projStatementCount = 0;
    foreach my $ent (@sorted) {
    
    
      #just use the filename if there is no other file by that name in the project to ensure the most matches
      #if there are more files with the name, walk up the directory structure until there is a unique name
      my $relname = $ent->relname();
      my $name = $ent->simplename();
      my @matches = $db->lookup($name,$kind,1);
      while(@matches > 1 && $name ne $relname){
        my $tempname = $relname;
        $tempname =~ /[\/\\]([^\/\\]+[\/\\]\Q$name\E)/;
        $name = $1;
        my @newmatches;
        foreach my $match (@matches){
          push @newmatches,$match if $match->relname =~ /\Q$name\E/;
        }
        @matches=@newmatches;
      }
      $relname = $name;


	# show progress
	if ($opts{files}) {
	    print STDOUT "$relname\n";
	}
	else {
	    print STDOUT ".";
	}


	# get statements or lines of code, etc
	my $statements;
	if ($opts{comp} =~ /comments/i) {
	    $statements = getComments($ent);
	}
	if ($opts{comp} =~ /lines/i) {
	    $statements = getLines($ent);
	}
	if ( $opts{comp} =~ /loc/i ) {
	    $statements = getLinesOfCode($ent);
	}
	if ( $opts{comp} =~ /statements/i ) {
	    $statements = getStatements($ent);
	    # sort by chunk id
	    my @sortedStatements = sort { $a->id() <=> $b->id(); } @$statements;
	    $statements = \@sortedStatements;
	}
	if ( $opts{comp} =~ /blank/i ) {
		$statements = getBlankLines($ent);
	}
	if ( $opts{comp} =~ /functions/i ) {
		$statements = getFunctions($ent);
	}



        # push info about this file on to the array
        my $details = FileInfo->new(
				    longname   => $ent->longname(),
				    relname    => $relname,
				    shortname  => $ent->name(),
				    statements  => $statements,
				);

	# save file metrics
	if ( $opts{metrics} ) {
	    foreach my $key ($ent->metrics()) {
        $details->{metrics}->{$key} = $ent->metric($key);
	    }
	}
  
  #save architecture metrics
  if ($opts{perarch}){
      foreach my $arch ($db->archs($ent)){
        next if $arch->longname =~ /^Directory Structure/;
        $details->{archs}->{$arch->longname}=$arch->longname;
        $archs{$arch->longname}{ent}=$arch;
        
      }
    }
  
  
	# show me the statements (chunks) that will be used in the comparison
	if ( $opts{chunks} ) {
	    print "\nStatements list for ", $ent->longname(), ":\n";
	    foreach my $statement (@$statements) { 
        print "chunk[". $statement->id() . "]: =>". $statement->text() . "<=\n";
	    }
	    print "\n";
	}


	$projStatementCount += $details->statementCount();
	push (@{$files->{$db_file}},$details);
    }

    # show progress
    print STDOUT "\n";

    # close database
    $db->close() unless $isGUI;

    # return project file and statement count
    return( scalar(@sorted), $projStatementCount );

}




sub getComments
{
    my $fileEnt = shift;
    my $filename = $fileEnt->longname();

    my $statements = [];

    # create lexer object
    my ($lexer,$status) = $fileEnt->lexer();
    die "\ncan't open lexer on file $filename, error: $status" if ($status);

    # scan lexemes
    my $lexemeIdx;
    my $statementId = 0;
    foreach my $lexeme ( $lexer->lexemes() ) {

	if ( $lexeme->token() =~ /Comment/g ) {
	    my $details = Statement->new(
					 text   => $lexeme->text(),
					 line   => $lexeme->line_begin(),   
					 col   => $lexeme->column_begin(),   
					 id   => ++$statementId,
					 );
	    push( @{$statements}, $details );
	}
    }

    return($statements);
}


sub getBlankLines
{
	my $fileEnt = shift;
    my $filename = $fileEnt->longname();

    my $statements = [];

    # create lexer object
    my ($lexer,$status) = $fileEnt->lexer();
    die "\ncan't open lexer on file $filename, error: $status" if ($status);

	# scan lexemes
	my $blank = 1;
    my $statementId = 0;
    foreach my $lexeme ( $lexer->lexemes() ) {
    	if ( $lexeme->token() eq "Newline" ) {
    		if ($blank) {
				my $details = Statement->new(
							 text   => "",
							 line   => $lexeme->line_begin(),
							 col   => 1,
							 id   => ++$statementId,
							 );
				push( @{$statements}, $details );
			}
			
			$blank = 1;
		} elsif ( $lexeme->token() ne "Whitespace" ) {
			$blank = 0;
		}
    }

    return($statements);
}


sub getLinesOfCode
{
    my $fileEnt = shift;
    my $filename = $fileEnt->longname();

    my $statements = [];

    # create lexer object
    my ($lexer,$status) = $fileEnt->lexer();
    die "\ncan't open lexer on file $filename, error: $status" if ($status);

    # scan lexemes
    # slurp lines with comments extracted, omit empty lines
    my $text;
    my $line;
    my $column;
    my $statementId = 0;
    my $preproc;
    my $isCode;
    foreach my $lexeme ( $lexer->lexemes() ) {

	# save non-blank strings when newline is encountered
	if ( $lexeme->token() eq "Newline" ) {
	    if ( $isCode && !$preproc) {
		my $statement = Statement->new(
					       text   => $text,
					       line   => $line,
					       col   => $column,
					       id   => ++$statementId,
					     );
		push( @{$statements}, $statement );
	    }
	    # clear text
	    $preproc = 0;
	    $isCode = 0;
	    $text = "";
	    next;
	}

	# append to text if code, skipping all comments
	if (!$lexeme->inactive) {

	    # remember where it begins
	    if ( !length($text) ) {
			$line = $lexeme->line_begin();
			$column = $lexeme->column_begin();
	    }
	    $text .= $lexeme->text();
	    $preproc=1 if $lexeme->token() eq "Preprocessor";
	    $isCode=1 if isCode($lexeme->token());
	}
	
    }
    return($statements);
}

sub isCode
{
	my $lexeme = shift;
	return ($lexeme eq "Identifier" || $lexeme eq "Keyword" || $lexeme eq "Literal" || $lexeme eq "Operator" || $lexeme eq "Punctuation" ||$lexeme eq "String");
}
sub getLines
{
    my $fileEnt = shift;
    my $filename = $fileEnt->longname();

    my $fh;
    my $statements = [];
    open($fh,"< $filename") or die "\ncan't open file $filename\n";
    my $line;
    my $statementId = 0;
    while (<$fh>) {
	$line++;
	my $text = $_;
	chomp($text);
	my $details = Statement->new(
				     text   => $text,
				     line   => $line,
				     col   => 1,
				     id   => ++$statementId,
				     );
	push( @{$statements}, $details );
    }
    close($fh);
    
    return($statements);
}

sub getFunctions
{
	my $fileEnt = shift;
	
	my $statements = [];
	my $statementId = 0;

	my $lexer = $fileEnt->lexer();
	my @funcRefs = $fileEnt->filerefs("define", "function, method ~constructor");
	foreach my $funcRef (@funcRefs) {
		my $endRef = $funcRef->ent->ref("end");
		my @lexemes = $lexer->lexemes($funcRef->line, $endRef->line);
		
		my $text;
		foreach my $lexeme (@lexemes) {
			$text .= $lexeme->text;
		}
		
		my $details = Statement->new(text   => $text,
									 line   => $funcRef->line,
									 col    => 1,
									 id     => ++$statementId);
		
		push( @{$statements}, $details );
	}
	
	return($statements);
}


#
# statement info from c++ book
#
#   labeled-statement:
#          identifier : statement
#          case constant-expression : statement
#          default : statement
#          
#   expression-statement
#          expression(opt) ; 
#
#   compound-statement (or block)
#          { statement-list(opt) } 
#
#          statement-list:
#                 statement
#                 statement-list statement
#
#   selection-statement
#         if ( expression ) statement
#         if ( expression ) statement else statement
#         switch ( expression ) statement 
# 
#   iteration-statement
#         while ( expression ) statement
#         do statement while ( expression ) ;
#         for ( for-init-statement expression(opt) ; expression(opt) ) statement
#        
#         for-init-statement:
#                 expression-statement
#                 declaration-statement
#   jump-statement
#         break;
#         continue;
#         return expression(opt);
#         goto identifier;
#
#   declartation-statement
#         declaration
#
#   
#
#       primary-expression:
#             literal
#             this
#             :: identifier
#             :: operator-function-name
#             :: qualified-name
#             ( expression )
#             name
#
# 
#       declaration: 
#             decl-specifiers(opt) declaration-list(opt);
#             asm-declaration
#             function-declaration
#             linkage-declaration
#
#       decl-specifier:
#             storage-class-specifier
#             type-specifier
#             function-specifier
#             template-specifier
#             friend
#             typedef
#
#       decl-specifiers:
#             decl-specifiers(opt) decl-specifier
#
#       function-specifier:
#             inline
#             virtual
#
#       type-specifier:
#             simple-type-name
#             class-specifier
#             enum-specifier
#             elaborated-type-specifier
#             :: class-name
#             const
#             volatile
#
#   ... p520, C++ book for more.


sub getStatements
{
    my $ent = shift;
    my ($file,$endLine,$lineNum,$definein,$lexer,$status,$stream);
   
   if ($ent->kind->check("file")){
    $file = $ent;
    $endLine = 50000;
    $lineNum = 1;
  }else{
    $definein = $ent->ref("definein, body declarein");
    $file = $definein->file;
    $lineNum = $definein ? $definein->line():1;
    $endLine = $ent->ref("end")? $ent->ref("end")->line():50000;
  }

  ($lexer,$status) = $file->lexer(1,2,0,0);
  die "Could not create lexer for ".$file->name.": $status" unless $lexer;
  $stream = $lexer->lexeme($lineNum, 0);
    # scan whole lexer stream looking for statements

    my $working = [];
    my $statement;
    my $inPreprocessor;
    my $inLabel;
    my $inCommand;
    my $statementId = 0;
    my $statements = [];
    my $inForStatement;

    # create lexer object
    my ($lexer,$status) = $file->lexer();
    die "\ncan't open lexer on file ".$file->longname.", error: $status" if ($status);
    


    # convenience functions
    my $newStatement = sub {
      my $lexeme = shift;
      return (Statement->new(
                 text => $lexeme->text(),
                 line => $lexeme->line_begin(),   
                 col  => $lexeme->column_begin(),   
                 id   => ++$statementId,
                 ));
    };
    my $newEmptyStatement = sub {
      my $lexeme = shift;
      return (Statement->new(
                 text => "",
                 line => $lexeme->line_begin(),   
                 col  => $lexeme->column_begin(),   
                 id   => ++$statementId,
                 ));
    };
    my $finishStatement = sub {
      my $stmt = shift;
     if ( $stmt  && $stmt->text() =~ /\S/ && $stmt->text !~ /^\s*;\s*$/
        && $stmt->text() !~ /^\s*else\s*{}\s*$/ && $stmt->text !~ /^\s*{}\s*$/) {
          push(@{$statements}, $stmt);
      }
      return(undef);
    };
    my $pushWorking = sub {
      my $stmt = shift;
      push(@$working,$stmt);
    };
    my $popWorking = sub {
      my $stmt = pop(@$working);
      return($stmt);
    };


    while ($stream && $stream->line_end <= $endLine){
      
      my $token = $stream->token();
      my $text = $stream->text();
      next if $stream->inactive();
      
      my $keyword = 1       if ($token eq "Keyword");
      my $comment = 1       if ($token eq "Comment");
      my $operator = 1      if ($token eq "Operator");
      my $punctuation = 1   if ($token eq "Punctuation");
      my $newline = 1       if ($token eq "Newline");
      my $catch_stmt = 1    if ($keyword && $text eq "catch");
      my $opening_brace = 1 if ($punctuation && $text eq "{");
      my $closing_brace = 1 if ($punctuation && $text eq "}");
      my $semicolon = 1     if ($punctuation && $text eq ";");
      my $ternary = 1       if ($operator && $text eq "?");
      my $closeParen =1     if ($punctuation && $text eq ")");
      my $openParen =1      if ($punctuation && $text eq "(");

      # turn on inPreprocessor if token indicates so
      # and then off whenever we see a newline
      $inPreprocessor = 1 if ( $token eq "Preprocessor" );
      $inPreprocessor = 0 if ( $newline );
      $inForStatement = 1 if ($keyword && $text eq "for");

      # no statements in comments nor preprocessor
      next if ($comment || $inPreprocessor);

      # remember if we've seen a label on this line
      $inLabel = 1 if ( $operator && $text eq ":" );
      
      if ($keyword && $text =~ /^if$|^while$/ && $ent->language != "Ada"){
        slurpCondition(\$stream,\$statement);
        $statement = &$finishStatement($statement);
        next;
        
      }

      # finish statement at newline if label introduced earlier
      if ( $inLabel && $newline && $ent->language != "Java") {
          $statement = &$finishStatement($statement);
      }

      $inLabel = 0 if ($newline);
      next if ($newline);
      
      if ( !$statement) {
          # none defined, create new statement
          $statement = &$newEmptyStatement($stream);
      }
      if ( $opening_brace  && ! $inForStatement) {
          # push current, create new, push it 
          # '{' and '}' will be a statement unto themselves
          $inCommand =0;
          $statement->append($text);
          &$pushWorking($statement);
          $statement = undef;
          next;
      }elsif ( $semicolon && ! nextItemAdaParameter($stream, $ent)) {
          # append text to current, finish with current, no new 
          # Unless empty
            $statement->append($text);
            $statement = &$finishStatement($statement);
          next;
      }elsif ( $closing_brace ) {
          # pop previous, append text to it, finish with it, no new
          if($inForStatement){
           $inForStatement = 0;
           next;
          }
          $statement = &$popWorking();
          next if (!$statement);
          $statement->append($text);
          $statement = &$finishStatement($statement);
          next;
      }elsif( $catch_stmt){
        my $try = pop @$statements;
         $statement->append($text); 
      }else{
          # none of the above, append to current 
          $statement->append($text);
      }

    }continue{
      $stream=$stream->next if $stream;
    }

    # finish last
    $statement = &$finishStatement($statement);

    # move remainder to statements array, unfinished
    foreach $statement (@$working) { 
      $statement = &$popWorking();
      &$finishStatement($statement);
    }

    # my $count = 1;
    # foreach my $stmt (sort{$a->line <=> $b->line;}@{$statements}){
      # my $text = $stmt->text;
      # $text =~ s/\s+/ /g;
      # print "Stmt ".$count++.", Line ".$stmt->line.": ".$text."\n";
    # }
    return $statements;
}


#Verify that the next word is not an Ada parameter declaration
sub nextItemAdaParameter{
  my $stream = shift;
  my $ent = shift;
  return unless $ent->language() =~ /Ada/;
  return unless $stream;
  $stream = $stream->next;
  while ($stream && $stream->token =~ /Newline|Whitespace/){
    $stream = $stream->next;
  }
  return unless $stream;
  return unless $stream->ref();
  return unless $stream->ref()->kind->check("declarein body");
  return unless $stream->ent();
  return unless $stream->ent()->kind()->check("parameter");
  return 1;
}


sub getStatementsFromMetrics {
	my $file = shift;
	
	my @statements;
	my $statementId = 0;
	
	my $stmts = $file->metric("CountStmt");	
	for (my $i = 0; $i < $stmts; $i++) {
		my $statement = Statement->new(text => "", line => 0, col => 0,
		                               id => ++$statementId);
		push(@statements, $statement);
	}
	
	return \@statements;
}


sub noStatement
{
    my ($fileEnt,$lexeme) = @_;

    my $file = $fileEnt->name();
    my $text = $lexeme->text();
    my $line = $lexeme->line_begin();   
    my $col  = $lexeme->column_begin();   

    #print "mismatch: text=$text, file=$file, line=$line, col=$col\n";
}


#Start at a conditional keyword (if, while, do)
#End after the closing paren
sub slurpCondition{
  my $stream = shift;
  my $statement = shift;
  
  $$statement->append($$stream->text);
  $$stream = $$stream->next;
  
  while($$stream && $$stream->token =~ /Whitespace|Comment|Newline/){
    $$statement->append($$stream->text);
    $$stream = $$stream->next;
  }
  my $parenCount = 1;
  $$statement->append($$stream->text);
  $$stream = $$stream->next;
  
  while($$stream && $parenCount){
    $$statement->append($$stream->text);
    if ($$stream->token eq "Punctuation"){
      $parenCount++ if $$stream->text eq "(";
      $parenCount-- if $$stream->text eq ")";
    }
    $$stream = $$stream->next;
  }

  
}


# packages



{
package DiffInfo;
sub new
{
    my $package = shift;
    return bless { added => 0, 
		   changed => 0, 
		   deleted => 0, 
		   text => '', 
		   @_ }, $package;
}
sub added
{
    my $self = shift;
    return( @_ ? ($self->{'added'} = shift) : $self->{'added'} );
}
sub changed
{
    my $self = shift;
    return( @_ ? ($self->{'changed'} = shift) : $self->{'changed'} );
}
sub deleted
{
    my $self = shift;
    return( @_ ? ($self->{'deleted'} = shift) : $self->{'deleted'} );
}
sub text
{
    my $self = shift;
    return( @_ ? ($self->{'text'} = shift) : $self->{'text'} );
}
} # end of package DiffInfo



{
package FileInfo;
# member description
# 
# statements:   this is a reference to a hash of statement text
#               the key is the concatenation of line/column number of the start of the text
#               in the file as determined via lexer calls.
#
#    statement->{'$line.$col'} = text 
#
#
sub new
{
    my $package = shift;
    return bless { longname => '', 
		   relname => '', 
		   shortname => '', 
		   metrics => {}, 
		   statements => [],
		   diffs => {},
       archs => {},
		   @_ }, $package;
}
sub archs
{
    my $self = shift;
    return( @_ ? ($self->{'archs'} = shift) : $self->{'archs'} );
}
sub diffs
{
    my $self = shift;
    return( @_ ? ($self->{'diffs'} = shift) : $self->{'diffs'} );
}
sub longname
{
    my $self = shift;
    return( @_ ? ($self->{'longname'} = shift) : $self->{'longname'} );
}
sub metrics
{
    my $self = shift;
    return( @_ ? ($self->{'metrics'} = shift) : $self->{'metrics'} );
}
sub relname
{
    my $self = shift;
    return( @_ ? ($self->{'relname'} = shift) : $self->{'relname'} );
}
sub shortname
{
    my $self = shift;
    return( @_ ? ($self->{'shortname'} = shift) : $self->{'shortname'} );
}
sub statements
{
    my $self = shift;
    return( @_ ? ($self->{'statements'} = shift) : $self->{'statements'} );
}
sub statementCount
{
    my $self = shift;
    return( $self->{statements} ? scalar(@{$self->{statements}}) : 0);
    #return( scalar(@{$self->{statements}}));
}

} # end of package FileInfo




{
package Statement;

sub new
{
    my $package = shift;
    return bless { text => '', 
		   line => '', 
		   col => '', 
		   id => 0,
		   @_ }, $package;
}
sub append
{
    my $self = shift;
    $self->{'text'} .= shift;
}
sub col
{
    my $self = shift;
    return( @_ ? ($self->{'col'} = shift) : $self->{'col'} );
}
sub id
{
    my $self = shift;
    return( $self->{'id'} );
}
sub line
{
    my $self = shift;
    return( @_ ? ($self->{'line'} = shift) : $self->{'line'} );
}
sub text
{
    my $self = shift;
    return( @_ ? ($self->{'text'} = shift) : $self->{'text'} );
}

sub setText
{
  my $self = shift;
  $self->{'text'} = shift;
}

} # end of package Statement


# save for colorizing changes on a line
#	MATCH => sub { print "$listA->[shift]<br>\n"},
#	DISCARD_A => sub { print "<s><font color=red>$listA->[shift]</font></s><br>\n"},
#	DISCARD_B => sub { print "<u><font color=green>$listB->[shift,shift]</font></u><br>\n"},
    


########
# Package Hunk. A Hunk is a group of Blocks which overlap because of the
# context surrounding each block. (So if we're not using context, every
# hunk will contain one block.)
{
package Hunk;

sub new {
# Arg1 is output from &LCS::diff (which corresponds to one Block)
# Arg2 is the number of items (lines, e.g.,) of context around each block
#
# This subroutine changes $File_Length_Difference
#
# Fields in a Hunk:
# blocks      - a list of Block objects
# start       - index in file 1 where first block of the hunk starts
# end         - index in file 1 where last block of the hunk ends
#
# Variables:
# before_diff - how much longer file 2 is than file 1 due to all hunks
#               until but NOT including this one
# after_diff  - difference due to all hunks including this one
    my ($class, $piece, $context_items) = @_;

    my $block = new Block ($piece); # this modifies $FLD!

    my $before_diff = $File_Length_Difference; # BEFORE this hunk
    my $after_diff = $before_diff + $block->{"length_diff"};
    $File_Length_Difference += $block->{"length_diff"};

    # @remove_array and @insert_array hold the items to insert and remove
    # Save the start & beginning of each array. If the array doesn't exist
    # though (e.g., we're only adding items in this block), then figure
    # out the line number based on the line number of the other file and
    # the current difference in file lenghts
    my @remove_array = $block->remove;
    my @insert_array = $block->insert;
    my ($a1, $a2, $b1, $b2, $start1, $start2, $end1, $end2);
    $a1 = @remove_array ? $remove_array[0 ]->{"item_no"} : -1;
    $a2 = @remove_array ? $remove_array[-1]->{"item_no"} : -1;
    $b1 = @insert_array ? $insert_array[0 ]->{"item_no"} : -1;
    $b2 = @insert_array ? $insert_array[-1]->{"item_no"} : -1;

    $start1 = $a1 == -1 ? $b1 - $before_diff : $a1;
    $end1   = $a2 == -1 ? $b2 - $after_diff  : $a2;
    $start2 = $b1 == -1 ? $a1 + $before_diff : $b1;
    $end2   = $b2 == -1 ? $a2 + $after_diff  : $b2;

    # At first, a hunk will have just one Block in it
    my $hunk = {
	    "start1" => $start1,
	    "start2" => $start2,
	    "end1" => $end1,
	    "end2" => $end2,
	    "blocks" => [$block],
              };
    bless $hunk, $class;

    $hunk->flag_context($context_items);

    return $hunk;
}

# Change the "start" and "end" fields to note that context should be added
# to this hunk
sub flag_context {
    my ($hunk, $context_items) = @_;
    return unless $context_items; # no context

    # add context before
    my $start1 = $hunk->{"start1"};
    my $num_added = $context_items > $start1 ? $start1 : $context_items;
    $hunk->{"start1"} -= $num_added;
    $hunk->{"start2"} -= $num_added;

    # context after
    my $end1 = $hunk->{"end1"};
    ####$num_added = ($end1+$context_items > $#f1) ?
    ####              $#f1 - $end1 :
    ####              $context_items;
    $num_added = $context_items;
    $hunk->{"end1"} += $num_added;
    $hunk->{"end2"} += $num_added;
}

# Is there an overlap between hunk arg0 and old hunk arg1?
# Note: if end of old hunk is one less than beginning of second, they overlap
sub does_overlap {
    my ($hunk, $oldhunk) = @_;
    return "" unless $oldhunk; # first time through, $oldhunk is empty

    # Do I actually need to test both?
    return ($hunk->{"start1"} - $oldhunk->{"end1"} <= 1 ||
            $hunk->{"start2"} - $oldhunk->{"end2"} <= 1);
}

# Prepend hunk arg1 to hunk arg0
# Note that arg1 isn't updated! Only arg0 is.
sub prepend_hunk {
    my ($hunk, $oldhunk) = @_;

    $hunk->{"start1"} = $oldhunk->{"start1"};
    $hunk->{"start2"} = $oldhunk->{"start2"};

    unshift (@{$hunk->{"blocks"}}, @{$oldhunk->{"blocks"}});
}



sub output_old_diff {
# Note that an old diff can't have any context. Therefore, we know that
# there's only one block in the hunk.
    my ($hunk, $fileref1, $fileref2) = @_;
    my %op_hash = ('+' => 'a', '-' => 'd', '!' => 'c');

    my @blocklist = @{$hunk->{"blocks"}};
    warn ("Expecting one block in an old diff hunk!") if scalar @blocklist != 1;
    my $block = $blocklist[0];
    my $op = $block->op; # +, -, or !

    # count differences
    my ($added,$changed,$deleted);
    my (@list, @list1, @list2);
    @list1 = @$fileref1[$hunk->{"start1"}..$hunk->{"end1"}];
    @list2 = @$fileref2[$hunk->{"start2"}..$hunk->{"end2"}];
    
    if ($block->remove) {
	@list = @list1;
    }
    if ($block->insert) {
	@list = @list2;
    }
    # If a hunk is changed, but does not have the same number of lines,
    # report part of it as changed and part as added or deleted for metrics
    if ($op eq "!"){
        my $diff = scalar @list2 - scalar @list1; 
        if ($diff > 0){
            $added += $diff;
            $changed += scalar @list2 - $diff;
        }elsif ($diff < 0){
            $deleted += (-1 * $diff);
            $changed += scalar @list1 + $diff;
        }else{
            map { $changed++ } @list if ( $op eq "!" );
        }
    }
    map { $added++ }   @list if ( $op eq "+" );
    map { $deleted++ } @list if ( $op eq "-" );


    my $printChange = sub {
	my ($prefix,$statement) = @_;
	#print $prefix,  $statement->text(), "\n";
	$diffText .= $prefix . $statement->text() . "\n";
    };

    # print diffs block
    if ( $opts{diffs} ) {

	$diffText .= "--------\n";

	# Calculate item number range.
	# old diff range is just like a context diff range, except the ranges
	# are on one line with the action between them.
	my $range1 = $hunk->context_range(1);
	my $range2 = $hunk->context_range(2);
	my $action = $op_hash{$op} || warn "unknown op $op";
	$diffText .= "$range1$action$range2\n";
	
	# If removing anything, just print out all the remove lines in the hunk
	# which is just all the remove lines in the block
	if ($block->remove) {
	    my @outlist = @$fileref1[$hunk->{"start1"}..$hunk->{"end1"}];
	    map { &$printChange("< ",$_) } @outlist; # all lines will be '< text\n'
	}

	$diffText .= "---\n" if $op eq '!'; # only if inserting and removing
	if ($block->insert) {
	    my @outlist = @$fileref2[$hunk->{"start2"}..$hunk->{"end2"}];
	    map { &$printChange("> ",$_) } @outlist; # all lines will be '> text\n'
	}
    }

    return($added,$changed,$deleted);
}

sub context_range {
# Generate a range of item numbers to print. Only print 1 number if the range
# has only one item in it. Otherwise, it's 'start,end'
# Flag is the number of the file (1 or 2)
    my ($hunk, $flag) = @_;
    my ($start, $end) = ($hunk->{"start$flag"},$hunk->{"end$flag"});
    $start++; $end++;  # index from 1, not zero
    my $range = ($start < $end) ? "$start,$end" : $end;
    return $range;
}
} # end of package Hunk


########
# Package Block. A block is an operation removing, adding, or changing
# a group of items. Basically, this is just a list of changes, where each
# change adds or deletes a single item.
# (Change could be a separate class, but it didn't seem worth it)
{
package Block;
sub new {
# Input is a chunk from &Algorithm::LCS::diff
# Fields in a block:
# length_diff - how much longer file 2 is than file 1 due to this block
# Each change has:
# sign        - '+' for insert, '-' for remove
# item_no     - number of the item in the file (e.g., line number)
# We don't bother storing the text of the item
#
    my ($class,$chunk) = @_;
    my @changes = ();

# This just turns each change into a hash.
    foreach my $item (@$chunk) {
	my ($sign, $item_no, $text) = @$item;
	my $hashref = {"sign" => $sign, "item_no" => $item_no};
	push @changes, $hashref;
    }

    my $block = { "changes" => \@changes };
    bless $block, $class;

    $block->{"length_diff"} = $block->insert - $block->remove;
    return $block;
}


# LOW LEVEL FUNCTIONS
sub op {
# what kind of block is this?
    my $block = shift;
    my $insert = $block->insert;
    my $remove = $block->remove;

    $remove && $insert and return '!';
    $remove and return '-';
    $insert and return '+';
    warn "unknown block type";
    return '^'; # context block
}

# Returns a list of the changes in this block that remove items
# (or the number of removals if called in scalar context)
sub remove { return grep {$_->{"sign"} eq '-'} @{shift->{"changes"}}; }

# Returns a list of the changes in this block that insert items
sub insert { return grep {$_->{"sign"} eq '+'} @{shift->{"changes"}}; }

} # end of package Block


