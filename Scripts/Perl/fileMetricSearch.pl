# Understand Perl API Script '_file_metric_search.pl'
#
# Synopsis: Return metrics for all files that match a string
#
# Languages: All
#
# 
#
#
#  For the latest Understand perl API documentation, see
#      http://www.scitools.com/perl.html
#
#  29-May-2012 Kevin Groke (support@scitools.com)

sub usage($) {
    return shift(@_) . <<"END_USAGE";
Usage: $0 -db database
  -db database      Specify Understand database (required for
                    uperl, inherited from Understand)
  -s SearchString   Return the metrics for all files that match this string
END_USAGE
}


use Understand;
use Getopt::Long;
use strict;

my $dbPath;
my $help;
my $search;
GetOptions(
     "db=s" => \$dbPath,
     "help" => \$help,
     "s=s" => \$search,
          );

# help message
die usage("") if ($help);

# open the database
my $db=openDatabase($dbPath);

# Languages Selected: All


#code body*******************************************************************

my @ents = grep ($_->name =~ /\Q$search\E/,$db->ents("file"));
foreach my $ent (sort {lc($a->longname()) cmp lc($b->longname());} @ents){
  print $ent->longname."\n";
  my @metricsList = $ent->metrics();
  foreach my $metric (sort @metricsList){
    print "   $metric: ".$ent->metric($metric)."\n";
  }
}

#end body********************************************************************
closeDatabase($db);


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
    $db->close() if $dbPath;
}

# Pass in percent complete (decimal value) to update a progress bar in the GUI or command line
sub progress{
  my $percent = shift;
  if(Understand::Gui::db()){
    Understand::Gui::progress_bar($percent);
    Understand::Gui::yield();
  }else{
    print "Progress: ".int($percent*100)."%\r";
  }
}
