#
# Synopsis: Report dead code
#
# Categories: Coding Standards, Project Report
#
# Languages: Ada, Fortran, C, Java
#
# Description:
#   Report dead code. That is, report any functions or
#   other subprograms that are not directly or indirectly
#   called by one of the entities specified as used. If a
#   complete program is analyzed, this entity should be the
#   "main" entity.
#
#   For the latest Understand perl API documentation, see
#      http://www.scitools.com/perl/
#

sub usage($) {
  print shift,"\n";
  print "Usage: acjf_dead_code.pl -db database\n";
  print "  -db database      Specify Understand database (required for\n";
  print "                    uperl, inherited from Understand)\n";
  print "  -def              Optional, specify to have declaration location\n";
  print "                    displayed with each subprogram.\n";
  print "  -instances        Optional, include subprograms from instantiations\n";
  print "                    in analysis. By default, these are ignored.\n";
  print "  -root entities    Required, list of root functions known to be used\n";
  print "  -used             Optional, specify to have all used subprograms\n";
  print "                    reported, instead of unused ones.\n";
}

use Understand;
use Getopt::Long;
use strict;


# get options
my ($dbPath,$help,@root,$showDef,$showUsed,$showInstances);
GetOptions(
    "db=s"        => \$dbPath,
    "def"         => \$showDef,
    "help"        => \$help,
    "instances"   => \$showInstances,
    "root=s"      => \@root,
    "used"        => \$showUsed,
  ) or die usage("");
die usage("") if ($help);
die usage("Error, root entity(s) not specified") if !@root;
@root = split(/,/,join(',',@root));

# open the database
my $db=Understand::Gui::db();
if (!$db) {
  die usage("Error, database not specified") unless $dbPath;
  my $status;
  ($db,$status)=Understand::open($dbPath);
  die "Error opening database: $status\n" if $status;
}

# check language
my $language = $db->language();
die "$language is currently unsupported" if ($language !~ /ada|c|fortran|java/i);

# lookup root entities
my @pending = map {$db->lookup($_)} @root;
die "Error: no matching root entities" if !@pending;

# build initial list of unused entities (ie, all entities)
my ($ent,%unused);
foreach $ent ($db->ents(
    "function ~unresolved ~unknown ~pure,".
    "method ~unresolved ~unknown,".
    "procedure ~unresolved,".
    "program,".
    "subroutine ~unresolved")) {
  next if ($ent->library() =~ /standard/i);
  next if (!$showInstances && $ent->ref("instanceof"));
  if ($language =~ /c/i && ($ent->name() =~ "~|operator")) {
    push @pending,$ent;
    next;
  }
  next if ($language =~ /fortran/i && $ent->ref("UseRenameEntityby"));
  $unused{$ent->id()} = 1;
}

# remove root entities from unused
foreach $ent (@pending) {
  delete $unused{$ent->id()};
}

# expand each root entity, and subsequent pending entities
my %used;
while (my $ent = shift @pending) {
  $used{$ent->id()} = 1;
  foreach my $ref ($ent->refs("call,use")) {
    my $ins_ref = $ref->ent->ref("instanceof");
    if ($ins_ref) {
      addPending($ins_ref->ent());
    }
    if (exists $unused{$ref->ent->id()}) {
      addPending($ref->ent());
      if ($language =~ /c/i) {
        if ($ref->ent->type() =~ "virtual") {
          handleVirtual($ref->ent());
        } elsif ($ref->ent->kind->check("c virtual")) {
          handleVirtual($ref->ent());
        } elsif ($ref->ent->kind->check("member function")) {
          handleOverload($ref->ent());
        }
      }
    }
  }
}

# show requested output
if ($showUsed) {
  print "Used:\n";
  foreach $ent (sort {$a->longname() cmp $b->longname()}
                map {$db->ent_from_id($_)} keys %used) {
    print " ",$ent->longname();
    if ($showDef) {
      my $decl = getDeclRef($ent);
      if ($decl) {
        print "  [File: ", $decl->file()->longname(), " ",
              "Line: ", $decl->line(),"]";
      }
    }
    print "\n";
  }
} else {
  print "Unused:\n";
  foreach $ent (sort {$a->longname() cmp $b->longname()}
                map {$db->ent_from_id($_)} keys %unused) {
    print " ",$ent->longname();
    if ($showDef) {
      my $decl = getDeclRef($ent);
      if ($decl) {
        print "  [File: ", $decl->file()->longname(), " ",
              "Line: ", $decl->line(),"]";
      }
    }
  print "\n";
  }
}


# add an entity to the pending list
sub addPending {
  my $ent = shift;
  push @pending,$ent;
  delete $unused{$ent->id()};
}


# return declaration ref (based on language) or 0 if unknown
sub getDeclRef {
  my ($ent) =@_;
  my $decl;

  return $decl unless defined ($ent);

  # language-specific
  if ($language =~ /ada/i) {
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
      last if (($decl) = $ent->refs($type,"",1));
    }
  } elsif ($language =~ /fortran/i) {
    ($decl) = $ent->refs("definein","",1);
    ($decl) = $ent->refs("declarein","",1) unless ($decl);
  } elsif ($language =~ /java/i) {
    ($decl) = $ent->refs("definein","",1);
    ($decl) = $ent->refs("declarein","",1) unless ($decl);
  } else { # C/C++
    ($decl) = $ent->refs("definein","",1);
    ($decl) = $ent->refs("declarein","",1) unless ($decl);
  }
  return $decl;
}


# return the list of all derived classes (c++)
sub getDerivedClasses {
  my $parent = shift;
  my @derived = ();
  foreach my $ent ($parent->ents("c derive")) {
    push @derived,$ent;
    push @derived,getDerivedClasses($ent);
  }
  return @derived;
}


# return the list of methods, looked up by name, for a class
sub getMethods {
  my $ent = shift;
  my $name = shift;
  my @methods = ();
  foreach my $method ($ent->ents("c define,c declare","c member function")) {
    push @methods,$method if ($name eq $method->name())
  }
  return @methods;
}


# handle c++ overloaded methods
sub handleOverload {
  my $ent = shift;
  my ($parent) = $ent->ents("c declarein,c definein","c class,c struct");
  return if !defined $parent;
  my $name = $ent->name();
  foreach my $ent (getMethods($parent,$name)) {
    addPending($ent);
  }
}


# handle c++ virtual methods
sub handleVirtual {
  my $ent = shift;
  my ($parent) = $ent->ents("c declarein,c definein","c class,c struct");
  return if !defined $parent;
  my $name = $ent->name();
  foreach my $derived (getDerivedClasses($parent)) {
    foreach my $ent (getMethods($derived,$name)) {
      addPending($ent);
    }
  }
}
