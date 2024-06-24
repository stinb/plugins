#
# Synopsis: Exports graphic representations of specified functions.
#
# Categories: Graphic, Project Report, Entity Report
#
# Languages: All
#
sub usage {
    print shift,"\n";
    print "Usage: -db database [-ents file] [-kind kinds] [-format fmt] [-report type] [-dir dir]\n";
    print "  -db database     Specify Understand database if run from uperl.\n";
    print "  -ents file	      Read and create files only for the entities listed in file.\n";
    print "  -kind kinds      Only output information for the selected kinds.\n";
    print "                   A comma-separed list of kinds.  Defaults to:\n";
    print "                     Function, Procedure, Subroutine.\n";
    print "  -format fmt      Output graphics in specified format (jpeg, png, or visio).\n";
    print "  -report type     Specify what type of report to generate.\n";
    print "  -dir dir         Existing output directory to write files to.\n";
    print "  -norenames	      Do not generate graphics for renamed entities.\n";
    print "  -options         Specify graph specific options for each graph. Refer\n";
    print "  -variant         Specify graph variant\n";
    print "                   to the Perl API $ent->draw command for more details.\n";
    return 1;
}

my $abort=0;
sub error {
    print shift,"\n";
    $abort = 1;
    return 1;
}


use Understand;
use Getopt::Long;
use strict;

report();
sub report {
    return if parse_args();
    return if open_db();
    export_graphics();
}


my ($argDb,$argHelp,$file,$kinds,$fmt,$type,$dir,$norenames,$user_options,$user_variant);
sub parse_args {
    GetOptions
	("db=s" => \$argDb,
	 "help" => \$argHelp,
	 "e|ents=s" => \$file,
	 "f|format=s" => \$fmt,
	 "k|kind=s" => \$kinds,
	 "r|report=s" => \$type,
	 "d|dir=s" => \$dir,
	 "nr|norenames" => \$norenames,
	 "o|options=s" => \$user_options,
	 "v|variant=s" => \$user_variant,
	 ) or return usage("");
    return usage("") if ($argHelp);
    return error("Cannot find file $file\n") if ($file && !-r $file);

    $kinds = "function, procedure, subroutine" unless $kinds;
    #($kinds =~ s/(?=,|$)/ \~unresolved/g) unless ($kinds =~ m/\~/);

    $type = "Calls" unless $type;

    $user_options = "" unless $user_options;
    $user_variant = "" unless $user_variant;

    $fmt = ".jpg" if ($fmt =~ m/jpe?g/i);
    $fmt = ".png" if ($fmt =~ m/png/i);
    $fmt = ".vdx" if ($fmt =~ m/visio|vdx/i);
    $fmt = ".svg" if ($fmt =~ m/svg/i);
    $fmt = ".png" unless ($fmt);

    $dir = "." unless $dir;
    $dir =~ s:/$::;			# strip trailing slash, as needed.
    return error("Cannot open $dir, or $dir is not a directory.\n") unless (-d $dir and -w $dir);
    return $abort;
}


my $db;
sub open_db {
    $db = Understand::Gui::db();
    if (!$db) {
	return usage("Error, database not specified") unless $argDb;
	my $status;
	($db,$status) = Understand::open($argDb);
	return error("Error opening database: $status") if $status;
    }
    return $abort;
}


# Setup a list of functions to dump pictures of
# To handle overloaded functions use the full Ada name.
sub export_graphics {
    my @functions;
    if ($file) {
	open (INFILE, "$file") or return error("Could not open $file for reading: $!");
	foreach my $funcname (<INFILE>) {
	    $funcname =~ s/^\s*//;
	    $funcname =~ s/\s*$//;
	    push @functions, $db->lookup($funcname, $kinds);
	}
	close INFILE;
    } else {
	@functions = $db->ents($kinds);
    }
    
# if we're ignoring renames, cull them now.
    if ($norenames) {
	@functions = map {($_->refs("rename"))? () : $_ } @functions;
    }

# Get a list of every function and procedure. 
    my ($file, $output, %file_counts) = ();
    foreach my $func (@functions) {
	# this line makes sure that we generate a unique name for each 
	# file, so we won't overwrite previous files.
	$file = $func->longname();
	$file =~ y/.: \/\\/_/s;
	$file .= $file_counts{$file} if ($file_counts{$file}++);
	$file = $dir. "/" . $file . $fmt;

	$output = $func->draw($type, $file, $user_options, $user_variant);
	next if $output eq "Empty";
	print $func->longname()," -> ",$file;
	print ("\t", $output) if ($output);
	print "\n";
    }

    return $abort;
}

