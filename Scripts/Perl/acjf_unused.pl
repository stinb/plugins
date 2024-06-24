#
# Synopsis: Report unused entities
#
# Categories: Coding Standards, Project Report
#
# Languages: Ada, Fortran, C, Java
#
# Description:
#  Reports unused entities, sorted either by filename (default) or by
#  entity kind.  The file/line location of the definition is also reported.  
#  When run within Understand (from Options->Run A Command), double click 
#  on the output line to visit the entity definition in the source. 
#  Requires an existing Understand database.
#
#  For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl/
#

sub usage($) {
    print shift,"\n";
    print "Usage: unused.pl -db database\n";
    print "  -db database    Specify Understand database if run from uperl\n";
    print "  -kind kindname  A kind text string. For instance Enumerator\n";
    print "  -byfile         (optional) Specify to sort output by file (default)\n";
    print "  -bykind         (optional) Specify to sort ooutput by entity kind\n";
    print "  -inactive       (optional) Specify to consider references in inactive code\n";
}

use Understand;
use Getopt::Long;
use strict;


my ($argByfile,$argBykind,$argDb,$argHelp,$argInactive,$argLoc, $argKind);
my ($db,$language);
my @unused = ();
report();


sub report() {
    GetOptions
	("db=s"     => \$argDb,
	 "byfile"   => \$argByfile,
	 "bykind"   => \$argBykind,
	 "loc"      => \$argLoc,
	 "kind=s"	    => \$argKind,
	 "help"     => \$argHelp,
	 "inactive" => \$argInactive,
	 ) or return usage("");
    return usage("") if ($argHelp);

    # open the database
    $db = Understand::Gui::db();
    if (!$db) {
	return usage("Error, database not specified") unless $argDb;
	my $status;
	($db,$status) = Understand::open($argDb);
	return print("Error opening database: $status\n") if $status;
    }

    # check language
    $language = $db->language();
    return print("$language is currently unsupported")
	if ($language !=/ada|c|fortran|java/i);

    # list the appropriate entity kinds based on language
    # get matching entities language-specific
    
    my $entitykinds;
    if ($argKind)
    {
      $entitykinds= $argKind;  
    }
    else
    {
      $entitykinds =
	"ada object ~task ~unknown ~unresolved,".
	"ada constant ~unknown ~unresolved,".
	"ada parameter,".
	"ada function ~unknown ~unresolved,".
	"ada procedure ~unknown ~unresolved,".
	"ada type ~unknown ~unresolved,".
	"fortran variable,".
	"fortran function ~unresolved ~intrinsic,".
	"fortran subroutine ~unresolved ~instrinsic,".
	"fortran type ~unknown,".
	"c object ~unknown ~unresolved,".
	"c parameter,".
	"c function ~unknown ~unresolved,".
	"c type ~unknown ~unresolved ~unnamed,".
	"java variable,".
	"java parameter,".
	"java method,".
	"java class ~anonymous,".
	"java interface";
    }
    
    # store definein/declarein references for each unused entity
    loadUnused($entitykinds);
    if ($argBykind) {
	reportUnusedByKind();
    } else {
	reportUnusedByFile();
    }
}


# if the two references occur at the same file/line/column, it's a duplicate
# note that ref kind is not considered here
sub duplicateRef {
    my ($ref1, $ref2) = @_;
    return 0 if (!$ref1 || !$ref2);
  
    if (($ref1->file()->longname eq $ref2->file()->longname()) and
	($ref1->line() == $ref2->line()) and
	($ref1->column()  == $ref2->column()) )
	{ return 1; }

    return 0;
}


# called by sort which supplies $a and $b vars
# compare file name/line/col of two entity references
sub fileCompare {
    return -1 if (!$a);
    return -1 if (!$b);

    my $name = $a->file()->longname() cmp $b->file()->longname();
    my $line = $a->line() <=> $b->line();
    my $col =  $a->column() <=> $b->column();

    return $name unless ($name == 0);
    return $line unless ($line == 0);
    return $col;
}

  
# return declaration ref (based on language) or 0 if unknown
sub getDeclRef {
    my ($ent) = @_;
    my $decl;

    return $decl unless defined ($ent);
    
    # language-specific
    if ($language =~ /ada/i) {
	($decl) = $ent->refs("declarein ~spec ~body ~instance ~formal ~incomplete ~private ~stub","",1);
	($decl) = $ent->refs("spec declarein","",1) unless ($decl);
	($decl) = $ent->refs("body declarein","",1) unless ($decl);
	($decl) = $ent->refs("instance declarein","",1) unless ($decl);
	($decl) = $ent->refs("formal declarein","",1) unless ($decl);
	($decl) = $ent->refs("incomplete declarein","",1) unless ($decl);
	($decl) = $ent->refs("private declarein","",1) unless ($decl);
	($decl) = $ent->refs("stub declarein","",1) unless ($decl);
    }
    elsif ($language =~ /fortran/i) {
	($decl) = $ent->refs("definein","",1);
	($decl) = $ent->refs("declarein","",1) unless ($decl);
    }
    elsif ($language =~ /java/i) {
	($decl) = $ent->refs("definein","",1);
	($decl) = $ent->refs("declarein","",1) unless ($decl);
    }
    else { # C/C++
	($decl) = $ent->refs("definein","",1);
	($decl) = $ent->refs("declarein","",1) unless ($decl);
    }
    return $decl;
}


# called by sort which supplies $a and $b vars
# compare two decl references by kind of scope entity
# if the same kind, compare file name/line/col of two entity references
sub kindCompare 
{  
    return -1 if (!$a);
    return -1 if (!$b);
     
    my $kindcmp = $a->scope()->kindname() cmp $b->scope()->kindname();
    return $kindcmp unless ($kindcmp == 0);

    my $name = $a->file()->longname() cmp $b->file()->longname();
    my $line = $a->line() <=> $b->line();
    my $col =  $a->column() <=> $b->column();

    return $name unless ($name == 0);
    return $line unless ($line == 0);
    return $col;
}


# find all unused entities and 
# store the decl reference for each unused entity in the global var @unused
sub loadUnused {
    my ($kinds) = @_;  # entity kinds string to look for
    foreach my $ent ($db->ents($kinds)) {
	next if $ent->library() =~ /standard/i;
	next if $ent->refs("ada rename");
	next if $ent->refs("java definein","java abstract method");
	next if $ent->kind->check("java method") && $ent->name() eq "main";
	my $inactive = ($argInactive)? "": "~inactive";
	if (! $ent->refs
	    ("associationby,".
	     "base,".
	     "callby $inactive,".
	     "derive,".
	     "java callby,".
	     "java castby,".
	     "java createby,".
	     "java dotrefby,".
	     "java extendby,".
	     "java implementby,".
	     "java modifyby,".
	     "java override,".
	     "java setby ~init,".
	     "java throwby,".
	     "java typedby,".
	     "java useby,".
	     "derivefrom,".
	     "instanceof,".
	     "modifyby,".
	     "refby,".
	     "renameby,".
	     "subtypefrom,".
	     "typedby,".
	     "useby $inactive,".
	     "usepackageby,".
	     "usetypeby,".
	     "withby")) {
	    my $declref = getDeclRef($ent);
	    next if (! $declref);         # unresolved entity - skip over it
	    # if not used, check for setbys - may be set where defined but otherwise unused
	    my $setOnDecl = 0;  
	    my @setbyref = ();
	    if ($ent->kind()->check("object, parameter, variable")) {
		@setbyref = $ent->refs("Setby");
		if ($#setbyref == 0) {    # exactly one set
		    $setOnDecl = 1 if (duplicateRef ($setbyref[0], $declref));
		}   
	    }
	    # if no setby references, or only one at the point of definition, call it unused   
	    push (@unused, $declref) if (($#setbyref < 0) || $setOnDecl);
	}  
    }
}


# sort @unused by file and output
sub reportUnusedByFile {
    my $lastfile = "";
    foreach my $ref (sort {fileCompare($a, $b);} @unused) {
	my $thisfile = $ref->file()->longname(); 
	if ($thisfile ne $lastfile) {
	    print ("\n", $thisfile, "\n");
	    $lastfile = $thisfile;
	}
	print "\t";
	print $ref->scope()->longname();
	print " \t[File: ". $ref->file()->longname(). " Line: ". $ref->line();
	my @endref = $ref->scope()->refs("end");
	print "-".$endref[0]->line() if (@endref);
	print " LOC: ", $ref->scope()->metric("CountLineCode") || 1 if ($argLoc);
	print "]\n";
    }
}


# sort @unused by kind and report
sub reportUnusedByKind {
    my $lastkind = "";
    foreach my $ref (sort {kindCompare($a, $b);} @unused) {
	my $thiskind = $ref->scope()->kindname(); 
	if ($thiskind ne $lastkind) {
	    print ("\n", $thiskind, "\n");
	    $lastkind = $thiskind;
	}
	print "\t";
	print $ref->scope()->longname()." \t[File: ".$ref->file()->longname()." Line: ", $ref->line();
	my @endref = $ref->scope()->refs("end");
	print "-".$endref[0]->line() if (@endref);
	print " LOC: ", $ref->scope()->metric("CountLineCode") || 1 if ($argLoc);
	print "]\n";
    }
}
