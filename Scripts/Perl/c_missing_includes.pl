#
# Synopsis: Report include files that couldn't be located
#
# Categories: Project Report,Coding Standards
#
# Languages: C
#
# Description:
#  Reports include files that are included but not found. Also list where they are
#  included in a double click visitable format.
#

sub usage {
    print shift,"\n";
    print "Usage: c_unfoundincludes.pl -db database\n";
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

my $db;

report();

sub report {
    return if parse_args();
    return if open_db();
    return if check_language();
    
    my $numFound = 0;
    foreach my $unresolvedIncludeFile ($db->ents("header file unknown"))
    {
      my $numRefs=0;
      foreach my $ref ($unresolvedIncludeFile->refs("includeby ~inactive"))
      { 
         print "Missing Includes:\n\n" if (!$numFound); 
         $numFound++ if (!$numRefs);
         $numRefs++;
         print $unresolvedIncludeFile->longname() . "\n" if ($numRefs);
         print "\t" . $ref->file->longname() . "(" . $ref->line() . ")" . "\n";
      }
     }  
    print "\nFound $numFound\n";        
    
}

my ($argCalls,$argDb,$argDef,$argHelp,$argExclude,$argExcludeHeader);
sub parse_args {
    GetOptions
	("db=s"      => \$argDb,
	 "help"      => \$argHelp,
	 ) or return usage("");
    return usage("") if ($argHelp);
    $argExclude = build_pattern($argExclude) if $argExclude;
    return $abort;
}


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


# check that the db language is supported
my $language;
sub check_language {
    $language = $db->language();
    return error("$language is currently unsupported by this script")
	if ($language !~ /c/i);
    return $abort;
}

