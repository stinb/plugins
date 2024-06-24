#
# Sample Understand PERL API Script
#
# Synopsis: Produces a list of function prototypes for an Ada package
#
# Categories: Entity Report
#
# Languages: Ada
#
# Usage:
sub usage ($)
{
    return $_[0] . << "END_USAGE";
Usage: $0 -db database [-package packages] [-calls] [-declares]
  -db database      Specify Understand database (required for
	            uperl, inherited from Understand)
  -package packages Specify one or more packages as a comma-
		    delimited list.
  -calls	    Show prototypes for the calls made in a 
		    package.
  -declares	    Show prototypes for functions declared in 
		    the package.  (default unless -calls is 
		    specified)
  -omit names	    Omit functions with the specified name, 
		    must be an exact regex match on the 
		    entire name.
  -nooperators	    Omit overloaded operators from the report.
  -longnames	    Display long (fully qualified) names rather 
		    than regular (short) names of parameters.
  -outfile <file>   Send output to a file, rather than STDOUT.
END_USAGE
}

#  For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl.html
# 
# 13-07-2002  - Evan Knop
#


use Understand;
use Getopt::Long;
use strict;

my ($dbPath, $calls, $declares, @packages, $outfile, $longnames, $omit, $nooperators, $help);
GetOptions(
	   "db=s"		=> \$dbPath,
	   "p|pack|packages:s"	=> \@packages,
	   "c|call|calls"	=> \$calls,
	   "d|declare|declares"	=> \$declares,
	   "l|long|longnames"	=> \$longnames,
 	   "omit=s"		=> \$omit,
	   "n|noop|nooperators"	=> \$nooperators,
	   "o|out|outfile=s"	=> \$outfile,
	   "help"		=> \$help,
	   );

die usage("") if ($help);

my $db = openDatabase($dbPath);

$declares = 1 unless ($calls or $declares);

sub name( $ ) {
    $_[0]->name();
}
if ($longnames) {
    *name = sub( $ ) {
	return $_[0]->longname() if $_[0]->kind()->check("Parameter");
	return $_[0]->name();
    };
}

if ($outfile) {
    open OUTFILE, ">$outfile" or die "Cannot open $outfile: $!";
    select OUTFILE;
}

@packages = split /\s*,\s*/, join(", ", @packages);

@packages = map { if (substr($_, 0, 1) ne '@') {$_} else {
		 open FILE, substr($_, 1); <FILE> }} @packages;

if (@packages) {
    @packages = map {chomp $_; $db->lookup($_, "package")} @packages;
} else {
    @packages = $db->ents("package");
}

my $functype = ($nooperators)? "function ~operator, procedure" : "function, procedure";

foreach my $pack (@packages) {
    my @functions = $pack->ents("declare", $functype);
    next unless scalar @functions;
    my @out = ();
    
    if ($declares) {
	foreach my $func (@functions) {
	    next if ($func->name() =~ m/^${omit}$/);
	    my (@params) = $func->ents("Declare", "Parameter");
	    next unless scalar (@params) or $func->type();
	    push @out, "\t", name($pack), ".", name($func), "\n";
#	    push @out, " (", $func->kind()->longname(), ")", "\n";
	    foreach my $param (@params) {
		push @out, "\t\t", name($param), " :\t", $param->type(), "\n";
	    }
	    if ($func->type()) {
		push @out, "\t\tReturns :\t", $func->type(), "\n";
	    }
	    push @out, "\n";
#	    print $func->kindname(), " ", $func->name(), " (",
#	          join(", ", map {$_->name()} $func->ents("define", "parameter")),
#	          ") return ", $func->type(), ";\n";
	}
	if (scalar @out) {
	    print "Internal ", name($pack), " Subprogram Parameters :\n\n", @out, "\n";
	}
    }
    
    @out = ();
    if ($calls) {
	my %secondaries;
	
	foreach my $func (@functions) {
	    foreach my $external ($func->ents("call", $functype)) {
		next if ($func->name() =~ m/^${omit}$/);
		my ($ext_package) = $external->ents("Declarein", "Package");
		next unless ($ext_package);
		next if ($ext_package->id() == $pack->id());
		$secondaries{$ext_package->id()}{$external->id()} = $external;
	    }
	}

	foreach my $pack_id (keys %secondaries) {
	    my $package = $db->ent_from_id($pack_id);
#	    push @out, "Calls to package ", name($package), " :\n";
	    foreach my $func_id (keys %{$secondaries{$pack_id}}) {
		my $func = $secondaries{$pack_id}{$func_id};
		my (@params) = $func->ents("Declare", "Parameter");
		next unless scalar (@params) or $func->type();
		push @out, "\t", name($package), ".", name($func), "\n";
#		push @out, " (", $func->kind()->longname(), ")", "\n";
		foreach my $param ($func->ents("Declare", "Parameter")) {
		    push @out, "\t\t", name($param), " :\t", $param->type(), "\n";
		}
		if ($func->type()) {
		    push @out, "\t\tReturns :\t", $func->type(), "\n";
		}
		push @out, "\n";
	    }
#	    push @out, "\n";
	}
	
#	foreach my $func (@functions) {
#	    foreach my $external ($func->ents("call")) {
#		next if ($secondaries{$external->uniquename()});
#		my ($ext_package) = $external->ents("declarein", "package");
#		next if ($ext_package->uniquename() eq $pack->uniquename());
#		
#		print "\t", $pack->name()
#		print $func->kindname(), " ", $external->name(), " (",
#		    join(", ", map {$_->name()} $func->ents("define", "parameter")),
#		    ") return ", $external->type(), "; -- package ", $ext_package->name(), "\n;";
#	    }
#	}
	if (scalar @out) {
	    print "External ", name($pack), " Subprogram Parameters :\n\n", @out, "\n";
	}	
    }
}

close OUTFILE if ($outfile);


# subroutines
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
