#
# Sample Understand perl API program
#
# Synopsis: Displays the info browser information for the selected entity.
#
# Categories: Entity Report
#
# Languages: All
#
# Usage:
sub usage($)
{
    return @_[0] . << "END_USAGE";
Usage: $0
    Displays the info. browser entity for the currently open instance of Understand.
END_USAGE
}

#
#  For the latest Understand perl API documentation, see 
#      http://www.scitools.com/perl/
#  Refer to the documenation for details on using the perl API 
#  with Understand and for details on all Understand perl API calls.
# 
#  12-10-01 Evan Knop
#
use Understand;
use strict;

my ($db, $status) = Understand::Gui::db();
die usage("Understand not open.\n") if ($status);

my $current = Understand::Gui::entity();
die usage("No entity selected\n") unless ($current);

print $current->ib();
