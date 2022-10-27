# Demonstration of the add_to_arch method
# Blog Article: https://blog.scitools.com/automating-a-custom-architecture/

use Understand;
$db = Understand::open("fastgrep.und");

foreach $file ($db->lookup("regsub.c","file")) {
  $file->add_to_arch("perl/test"); # perl/test doesn't exist yet
}

$arch = $db->lookup_arch("perl/test"); # $file->add_to_arch created it, so now it can be looked up
print $arch->name() . "\n";

foreach $file ($db->lookup("regexp.c","file")) {
  $file->add_to_arch($arch); # and you can add to the arch object directly instead of by a longname string
}
