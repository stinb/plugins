#
# Sample Understand perl API program 
#
# Synopsis: Outputs metrics for specified kinds of entities
#
# Categories: Project Metrics, Entity Metrics
#
# Languages: Ada, Fortran, C, Java
#
# Usage:
sub usage($) {
    return shift(@_) . <<"END_USAGE";
Usage: $0 -db database  [-list] [-kinds kinds] [-name names]
  -db database      Specify Understand database (required for
                    uperl, inherited from Understand)
  -list		    (optional) List available metrics from Understand 
		    database.  -list will not generate a detailed report.
  -kinds kinds      (optional) if not specified, project level metrics and
                    all entities that have metrics will be reported.  If 
		    specfied, a report will be generated for each kind, 
		    containing metrics for each element of that kind.
  -name names	    (optional) limits the output to only those entities 
		    matching one of the specified names.  Has no effect 
		    if the -list option is given.
  -metric metrics   (optional) limits the output to only metrics matching 
		    one of the elements in metrics.
  -adjust file      (optional) a file containing Perl regExp entries that will
                    generate a modified line-count by subtracting lines that
		    match the specified regExp from the output metrics. Each regExp
		    must be on a separate line.
  -verbose	    print metrics with a value of "0".  If "-list" is 
		    specified, prints the descriptions of the metrics as 
		    well as their names.
END_USAGE
}


# Examples:
#    metrics -db myproject.udc -kinds project,class
#    metrics -db myproj.uda    -list -kinds "package, file"
#    metrics -db myproj.udf    -metric "cyclomatic"
#
# Description:
#  Outputs all metrics for the specified kind of entities. 
#  Default is for all entities that have metrics.
#  Metrics will be reported in the order specified as options, with entities 
#  listed alphanumerically within each group.  
#  Requires an existing Understand database
#
#  For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl/
#  Refer to the documenation for details on using the perl API 
#  with Understand and for details on all Understand perl API calls.
# 

use Understand;
use Getopt::Long;
use strict;


#
# get options
#

my $dbPath;
my ($option, @options);
my ($help, $list, $verbose);
my (@names);
my (@metrics);
my ($adjFile);

GetOptions
    ("db=s" => \$dbPath,
     "forwhat|kinds:s" => \@options,
     "help" => \$help,
     "v|verbose" => \$verbose,
     "list|whatmetrics" => \$list,
     "name|names:s" => \@names,
     "metric|metrics|limit:s" => \@metrics,
	 "adjust:s" => \$adjFile);
die usage("") if ($help);
my @types;
if (@options) {
    $option = lc( join ",", @options );
    $option =~ s/\s*//g;             # remove any whitespace
    @types = split /,/, $option;     # split on comma delimiter
} else {
    @types = ();
}
@metrics = map {split / *, */, $_} @metrics;

my @adjList;
if($adjFile) {
	my $FILE_HANDLE;
	if(open($FILE_HANDLE,"<",$adjFile)) {
		my $content;
		while($content = <$FILE_HANDLE>) {
			$content =~ s/#.*$//;	# remove comments
			chomp($content);
			next if(length($content) == 0);
			push(@adjList,$content);
		} # end while($content = <$FILE_HANDLE>)

		close($FILE_HANDLE);
	}else {
		die "Unable to open adjustment file: $adjFile\n";
	} # end if(open($FILE_HANDLE,"<badcomments.txt"))
} # end if($adjust)

# open the database 
my $db=openDatabase($dbPath);
my $language = $db->language();

# do list or data, as requested
if ($list) {
    do_list();
} else {
    do_data();
}

# close the database
closeDatabase($db);


sub do_list {
    my (@db_kinds, %kh);
    foreach my $entity ($db->ents("~unresolved ~unknown")) {
	$kh{$entity->kind()->name()} = undef;
    }
    @db_kinds = keys %kh;
    
    if (filter("project", @types)) {
	print "Project Metrics:\n   ";
	print join("	\n   ", sort $db->metrics() ) ."\n";
    }
    
    foreach my $kind (sort {$a cmp $b} @db_kinds) {
	if (regfilter($kind, @types)) {
	    my ($rep) = $db->ents("$kind ~unresolved ~unknown");
	    if ($rep) {
		my @met = map { regfilter($_, @metrics)? $_:() } $rep->metrics();
		if (@met) {
		    print "-----------------------\n";
		    print $kind . ":\n   ";
		    if ($verbose) {
			@met = map {$_." - ".Understand::Metric::description($_)} @met;
		    }
		    print join("\n   ", @met ) . "\n";
		}
	    }
	}
    }
}


sub do_data {
    @names = map {split / *, */, $_} @names;
    do_all_metrics(@names) unless @types;

    foreach my $type (@types) {
	if ($type eq "project")	{do_project_metrics(); next; }
	# make sure we have the singular for understand...
	$_ = $type; s/([^s])s$/$1/i and s/se$/s/;
	do_entity_metrics ($type, "$_ ~unknown ~unresolved", 0, @names);
    } continue  {
	print "-----------------------\n";
    }
}


# see if an element is in a list, or the list is empty.
sub filter ($@) {
    my ($elem, @list) = @_;
    foreach my $test (@list) {
	return 1 if ($test eq lc($elem) );
    }
    return 0 if @list;
    return 1;
}


sub regfilter ($@) {
    my ($elem, @list) = @_;
    foreach my $test (@list) {
	$test =~ s/\\/\\\\/g;
	$test =~ s/\./\\\./g;
	$test =~ s/\*/\.\*/g;
	$test =~ s/\?/\./g;
	$test = "^$test\$";
	return 1 if ($elem =~ m/$test/i);
    }
    return 0 if @list;
    return 1;
}


# report project metrics
sub do_project_metrics {
   print "Project Metrics:\n\n";
   
	my $adjLineCount = 0;
	my $adjLineCountCode = 0;
	my $adjLineCountCmnt = 0;
	my $adjRatioCmntToCode = 0;
	if($adjFile) {
		GetAdjustedProjectCounts(\$adjLineCount,\$adjLineCountCode,
								 \$adjLineCountCmnt,\$adjRatioCmntToCode);
	} # end if($adjFile)
	
   my @metricslist = map { regfilter ($_, @metrics)? $_:() } $db->metrics();
   if (@metricslist) {
       my @metric_values = $db->metric(@metricslist);
       for (my $i=0; $i<scalar(@metric_values); $i++) {
	   print "        ".$metricslist[$i].": ".$metric_values[$i];
	   
		if($adjFile && (lc($metricslist[$i]) eq "countline")) {
			print("/".$adjLineCount);
		}elsif($adjFile && (lc($metricslist[$i]) eq "countlinecode")) {
			print("/".$adjLineCountCode);
		}elsif($adjFile && (lc($metricslist[$i]) eq "countlinecomment")) {
			print("/".$adjLineCountCmnt);
		}elsif($adjFile && (lc($metricslist[$i]) eq "ratiocommenttocode")) {
			print("/".$adjRatioCmntToCode);
		} # end if($adjFile && (lc($metricslist[$i]) eq "linecount"))
		print "\n";
       }
   } else {
       print "    No project data to report\n\n";  
   }
   print "\n";
}

sub GetAdjustedProjectCounts {
	my ($adjLineCount,$adjLineCountCode,$adjLineCountCmnt,$adjRatioCmntToCode) = @_;
	$$adjLineCount = $db->metric("CountLine");
	$$adjLineCountCode = $db->metric("CountLineCode");
	$$adjLineCountCmnt = $db->metric("CountLineComment");
	$$adjRatioCmntToCode = 0;
	
	foreach my $fileEnt ($db->ents("file ~unknown ~unresolved")) {
		my $lexer = $fileEnt->lexer();
		if($lexer) {
			foreach my $lexeme ($lexer->lexemes()) {
				if(lc($lexeme->token()) eq "comment") {
					foreach my $listItem (@adjList) {
						if($lexeme->text() =~ /$listItem/) {
							--$$adjLineCount;
							--$$adjLineCountCode;
							--$$adjLineCountCmnt;
							last;
						} # end if($lexeme->text() =~ /$listItem/i)
					} # end foreach my $listItem (@adjList)
				} # end if(lc($lexeme->token()) eq "comment")
			} # end foreach my $lexeme ($lexer->lexemes())
		} # end if($lexer)
	} # end foreach my $fileEnt ($db->ents("file ~unknown ~unresolved"))
	
	$$adjLineCount = 0 if($$adjLineCount < 0);
	$$adjLineCountCode = 0 if($$adjLineCountCode < 0);
	$$adjLineCountCmnt = 0 if($$adjLineCountCmnt < 0);
	$$adjRatioCmntToCode = ($$adjLineCountCmnt / $$adjLineCountCode)
		if($$adjLineCountCode > 0);
} # end sub GetAdjustedProjectCounts ()

# report metrics for project plus all entities
sub do_all_metrics(@names) {
   do_project_metrics() if (filter("project", @names));
   do_entity_metrics ("All Entity Metrics", "~unresolved ~unknown", 1, @names);
}


# report specified metric kinds
sub do_entity_metrics($$$@) {
    my ($header, $kind, $showdecl, @names) = @_;
    print "$header:\n\n";  

    # only include ents which match our names...
    my @ents = map { regfilter($_->longname(), @names)? $_:() } $db->ents($kind);

    if (@ents == 0) {  
	print "    No $header data to report\n\n";  
	return;
    }

    foreach my $ent (sort{$a->longname() cmp $b->longname();} @ents) {
	#print $ent->longname(). "\n";
      
	my @metricslist = map {regfilter($_, @metrics)? $_:() } $ent->metrics();
	if (@metricslist) {
	    my @metric_values = $ent->metric(@metricslist);
	    print "    ".$ent->kindname().": ".$ent->longname()." :";
	    if ($showdecl) {
		my @decl = getDeclEnt ($ent);
		if (@decl) {
		    print "   [File: ".$decl[0]->file()->longname()." Line: ".$decl[0]->line() ."]";
		}
	    }
	    print "\n";

		my $adjLineCount = 0;
		my $adjLineCountCode = 0;
		my $adjLineCountCmnt = 0;
		my $adjRatioCmntToCode = 0;
		my $showAdjusted = ($adjFile && ($ent->kindname() =~ /function|method|file/i));
		if($showAdjusted) {
			GetAdjustedEntityCounts($ent,\$adjLineCount,\$adjLineCountCode,
									 \$adjLineCountCmnt,\$adjRatioCmntToCode);
		} # end if($showAdjusted)

	    for (my $i=0; $i<scalar(@metric_values); $i++) {
			next if ( !$verbose and $metric_values[$i] == 0 );
			print "        ".$metricslist[$i].": ".$metric_values[$i];
			
			if($showAdjusted && (lc($metricslist[$i]) eq "countline")) {
				print("/".$adjLineCount);
			}elsif($showAdjusted && (lc($metricslist[$i]) eq "countlinecode")) {
				print("/".$adjLineCountCode);
			}elsif($showAdjusted && (lc($metricslist[$i]) eq "countlinecomment")) {
				print("/".$adjLineCountCmnt);
			}elsif($showAdjusted && (lc($metricslist[$i]) eq "ratiocommenttocode")) {
				print("/".$adjRatioCmntToCode);
			} # end if($showAdjusted && (lc($metricslist[$i]) eq "linecount"))
			print "\n";
	    }
	    print "\n";
	}
    }
}

sub GetAdjustedEntityCounts {
	my ($ent,$adjLineCount,$adjLineCountCode,$adjLineCountCmnt,$adjRatioCmntToCode) = @_;
	$$adjLineCount = $ent->metric("CountLine");
	$$adjLineCountCode = $ent->metric("CountLineCode");
	$$adjLineCountCmnt = $ent->metric("CountLineComment");
	$$adjRatioCmntToCode = 0;

	my $lexer;
	my @lexemes;
	if($ent->kindname() =~ /file/i) {
	   $lexer = $ent->lexer();
	   @lexemes = $lexer->lexemes();
	}else {
		my $defRef = $ent->ref("definein");
		my $endRef = $ent->ref("end");
		
		$lexer = $defRef->file()->lexer();
		@lexemes = $lexer->lexemes($defRef->line(),$endRef->line());
	} # end if($ent->kindname() =~ /file/i)
	
	if($lexer) {
		foreach my $lexeme (@lexemes) {
			if(lc($lexeme->token()) eq "comment") {
				foreach my $listItem (@adjList) {
					if($lexeme->text() =~ /$listItem/) {
						my $adjAmt = (($lexeme->line_end() - $lexeme->line_begin) + 1);
						$$adjLineCount -= $adjAmt;
						$$adjLineCountCode -= $adjAmt;
						$$adjLineCountCmnt -= $adjAmt;
						last;
					} # end if($lexeme->text() =~ /$listItem/i)
				} # end foreach my $listItem (@adjList)
			} # end if(lc($lexeme->token()) eq "comment")
		} # end foreach my $lexeme (@lexemes)
	} # end if($lexer)
			
	$$adjLineCount = 0 if($$adjLineCount < 0);
	$$adjLineCountCode = 0 if($$adjLineCountCode < 0);
	$$adjLineCountCmnt = 0 if($$adjLineCountCmnt < 0);
	$$adjRatioCmntToCode = ($$adjLineCountCmnt / $$adjLineCountCode)
		if($$adjLineCountCode > 0);
} # end sub GetAdjustedEntityCounts ()

# return ref where entity defined
sub getDeclEnt {
    my ($ent) = @_;    
    return () unless defined($ent);
    
    my @decl=();   
    if ($language =~ "Ada") {
	my @declOrder =
	    ("declarein ~spec ~body ~instance ~formal ~incomplete ~private ~stub",
	     "spec declarein",
	     "body declarein",
	     "instance declarein",
	     "formal declarein",
	     "incomplete declarein",
	     "private declarein",
	     "stub declarein");
	foreach my $type (@declOrder) {
	    @decl = $ent->refs($type);
	    if (@decl) { last;  }
	}
    } else {
        @decl = $ent->refs("Definein");
    }
    return @decl; 
}


sub openDatabase($) {
    my ($dbPath) = @_;
    my $db = Understand::Gui::db();

    # path not allowed if opened by understand
    if ($db&&$dbPath) {
	die "database already opened by GUI, don't use -db option\n";
    }

    # open database if not already open
    if (!$db) {
	my $status;
	die usage("Error, database not specified\n") unless ($dbPath);
	($db,$status)=Understand::open($dbPath);
	die "Error opening database: ",$status,"\n" if $status;
    }
    return($db);
}


sub closeDatabase($) {
    my ($db)=@_;
    # close database only if we opened it
    $db->close() if ($dbPath);
}
