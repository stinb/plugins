#
# Sample Understand perl API program 
#
# Synopsis:   Generates an elaboration order. Does not take elaboration pragmas
#			  into account.
#
# Categories: Project Report
#
# Languages: Ada
#
# Usage:
sub usage ($)
{
    return << "END_USAGE";
${ \( shift @_ ) }
Usage: a_elaboration_order -db database [-out file]
        -db database   Specify Understand database (required for 
                       uperl, inherited from Understand)
	 -out file     Specify output file instead of stdout (optional)
END_USAGE
}

use Understand;
use Getopt::Long;
use strict;


# get command line args 
my $dbPath;
my $outFile;
my $help;
GetOptions(
	   "db=s"  => \$dbPath,
           "out=s" =>\$outFile,
	   "help" => \$help,
);

# help message
die usage("") if ($help);

# open the database 
my $db=openDatabase($dbPath);

# check language
if ( $db->language() !~ "Ada" ) {
    die "This script is designed for Ada only\n";
}

# use optional output file
if ($outFile) {
    open(RPT, "> $outFile") or die "Can't open output file $outFile: $!\n";
    print "Writing output to $outFile\n";
    select(RPT);
}

#get all the libary items and add them to the lib_items hash
my %lib_items = ();
my @ents;

# get the library units
@ents = findRootUnits();
foreach my $ru (@ents) {
   my @spec_withs;
   my @body_withs;
   my @subunit_withs;
   my $has_spec = 0;
   my $long_name = $ru->longname();

   # check to see if this lib unit has a spec
   if ($ru->refs("declarein spec, declarein instance"))  {
      $has_spec = 1;
      # build the name
      my $spec_name = $long_name . "-spec";
      # get withs made by spec
      foreach my $ref ($ru->refs("spec with")) {
         next if ($ref->ent()->library() eq "Standard") or
                  $ref->ent()->kind()->check("unknown");
         my $w_name = getWithedName($ref->ent()); 
         push @spec_withs, $w_name;
      }
      # make dependency to parent lib unit 
      my $parent_unit = $ru->refs("parent libunit");
      if ($parent_unit and $parent_unit->ent()->library() ne "Standard" and
             not $parent_unit->ent()->kind()->check("unknown")) { 
         push @spec_withs, $parent_unit->ent()->longname() . "-spec";
      }
      $lib_items{$spec_name} = [ @spec_withs ];
   }
   # check to see if this lib unit has a body
   if ($ru->refs("declarein body")) {
      # build the name
      my $body_name = $long_name . "-body";
      # get withs made by body
      foreach my $ref ($ru->refs("body with")) {
         next if ($ref->ent()->library() eq "Standard" or
                  $ref->ent()->kind()->check("unknown"));
         my $w_name = getWithedName($ref->ent()); 
         push @body_withs, $w_name;
      }
      # make dependency from body to spec
      if ($has_spec) {
         push @body_withs, $long_name . "-spec";
      }
      # get any subunit withs
      @subunit_withs = getSubunitWiths($ru);
      foreach my $su_with (@subunit_withs) {
         push @body_withs, $su_with;
      }  
      $lib_items{$body_name} = [ @body_withs ];
   }
}


my @unit_deps;

#debug print 
#foreach my $li (sort keys %lib_items) {
#   @unit_deps = @{ $lib_items{$li} };
#   print $li . "\n";
#   print "   ";
#   foreach my $unit (@unit_deps) {
#      print "$unit ";
#   }
#   print "\n";
#}  
 
my @order_list = ();
my %ordered_hash = ();
my $item_placed = 1;

while ($item_placed) {
   $item_placed = 0;
   foreach my $li (keys %lib_items) {
      my $all_in_hash = 1;
      @unit_deps = @{ $lib_items{$li} };
      # check to see if all dependents are ordered 
      foreach my $unit (@unit_deps) {
         if (not $ordered_hash{$unit}) {
            $all_in_hash = 0;
         }
         last if not $all_in_hash;
      }
      if ($all_in_hash)  {
         $ordered_hash{$li} = "1";
         push @order_list, $li;
         delete $lib_items{$li};
         $item_placed = 1;
      }
      
   }
}
if (%lib_items) {
   print "\nUnable to establish order for:\n";
   foreach my $li (sort keys %lib_items) {
      print "   " . $li . "\n";
      @unit_deps = @{ $lib_items{$li} };
      print "      ";
      foreach my $unit (@unit_deps) {
         print "$unit ";
      }
      print "\n";
   }
} 


print "\n";
print "Elaboration Order:\n";
foreach my $ord_item (@order_list) {
   print "   $ord_item\n";
}

close (RPT);
closeDatabase($db);


# subroutines

# return list of all "root" entities
# ignoring any duplicates
sub findRootUnits
{
   my %roothash = ();
   
   my @allfiles = $db->ents("file ~unresolved");
   foreach my $file (@allfiles)
   {
      if ($file->library() ne "Standard") 
      {
         foreach my $root ( $file->refs("root", "",1)) 
         {  
            if ( not defined( $roothash {$root->ent()->id()}) )
            {
               $roothash {$root->ent()->id()} = $root->ent();
            }
         }
      }
   }
   return values (%roothash)
}


sub getWithedName
{
   my($ent) = @_;
   my $name;

   return unless defined($ent);

   if ($ent->kind()->check("function,procedure") &&
       !$ent->refs("declarein spec")) {
      $name =  $ent->longname() . "-body"; 
   }
   else {
      $name =  $ent->longname() . "-spec"; 
   }
   return $name;
} 

sub getSubunitWiths 
{
   my($u_ent) = @_;
   my @su_withs = ();
   my @nest_su_withs;
   

   my @su_refs = $u_ent->refs ("separate");
   foreach my $ref (@su_refs)  {
      foreach my $su_with ($ref->ent()->refs("with body"))  {
         next if ($su_with->ent()->library() eq "Standard" or
                  $su_with->ent()->kind()->check("unknown"));
         my $w_name = getWithedName($su_with->ent()); 
         push @su_withs, $w_name;
      }
      @nest_su_withs = getSubunitWiths($ref->ent());
      foreach my $n_with (@nest_su_withs) {
         push @su_withs, $n_with;
      }
   }
   return @su_withs;
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












