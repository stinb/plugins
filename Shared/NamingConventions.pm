package CodeCheck::Libraries::NamingConventions;
use strict;
use Exporter;
our @ISA	= qw(Exporter);
our @EXPORT = qw(setOptions registerNamingErrors getNamingDescription setNamingOptions checkNamingRules getPrimaryKind NAMEERROR);

our $kind;                #The display name for the tested entity kind
our $kindPlural;          # The display name for multiple entity, e.g. classes for class
our $languageText;        # The language name how it should be displayed
our $languageTest;        # The language name how it needs to appear when testing for compliance
our $canChangePrecedence; # Set to 1 if the entity kind has conflicting kinds that need a precedence
our $defaultAll;          # Set to 1 to default to displaying an All entites option first
our %kindNameLookup;      # The pretty name for each entity kind, this defaults the base, e.g. Global Variable
our @kindStrings;         # List the individual kinds for the entity


###############################################################


use constant NAMEERROR => '%1 name \'%2\' is invalid. %3';
use constant TOOSHORT => ' It is too short.';
use constant TOOLONG => ' It is too long.';
use constant MISSINGPREFIX => ' It is missing the required prefix.';
use constant BADPREFIX => ' It uses a disallowed prefix.';
use constant MISSINGSUFFIX => ' It is missing the required suffix.';
use constant BADSUFFIX => ' It uses a disallowed suffix.';
use constant MISSINGINITCAP=> ' It is missing the initial capital.';
use constant MISSINGINITLC=> ' It has a leading capital.';
use constant HASLOWERCASE=> ' It has lowercase letters.';
use constant HASUPPERCASE=> ' It has uppercase letters.';
use constant CONSECCAPS=>' It has consecutive capitals.';
use constant NONALPHANUM=>' It has characters other than letters and numbers.';
use constant NONALPHA=> ' It has characters other than letters.';
use constant BADREGEXP=> ' It matches the disallowed regular expression.';
use constant MISSINGREGEXP=> ' It does not match the required regular expression.';


sub setOptions{
  ($kind,$kindPlural,$languageText,$languageTest,$canChangePrecedence,$defaultAll) = @_;
    #setup default display names
  foreach (@kindStrings){
    if(! $kindNameLookup{$_}){
      $kindNameLookup{$_} = "$_ $kind";
    }
  }
     $kindNameLookup{$kind}=$kind if $defaultAll;
}

sub registerNamingErrors{
  my $check = shift;
  $check->add_tr_text(NAMEERROR);
  $check->add_tr_text(TOOSHORT);
  $check->add_tr_text(TOOLONG);
  $check->add_tr_text(MISSINGPREFIX);
  $check->add_tr_text(BADPREFIX);  
  $check->add_tr_text(MISSINGSUFFIX);
  $check->add_tr_text(BADSUFFIX); 
  $check->add_tr_text(MISSINGINITCAP);
  $check->add_tr_text(MISSINGINITLC);
  $check->add_tr_text(HASLOWERCASE);
  $check->add_tr_text(HASUPPERCASE);
  $check->add_tr_text(CONSECCAPS);
  $check->add_tr_text(NONALPHANUM);
  $check->add_tr_text(NONALPHA);
  $check->add_tr_text(MISSINGREGEXP);
  $check->add_tr_text(BADREGEXP);  
  foreach (values(%kindNameLookup)){
    $check->add_tr_text($_);
  }
}

sub getNamingDescription{

  my ($kind1,$kind2);
  if ($canChangePrecedence && @kindStrings>2){
      $kind1 = $kindStrings[0];
      $kind2 = $kindStrings[1];
  }
  
my $o;
$o .= "Select the required naming convention for <b>$languageText $kindPlural.</b>";
$o .= "<br>$kindPlural can have different naming conventions based off of their kind (@kindStrings)." if @kindStrings>1;
$o .= "<br>The first group of options will define the naming conventions for All $kindPlural, the other options will let you specify a subset of $kindPlural to test." if $defaultAll;
$o .= "<hr>";
$o .= "Naming Convention Options:<ul style=\"margin-top:0;padding-top:0;\">";
$o .= "<li><b>Precedence:</b> Set the precedence to determine the order that the rules will be applied to an entity that has multiple kinds. e.g. If $kind1 Precedence is set to 2 and $kind2 Precedence is 3, a $kind1 $kind2 $kind would be tested with the $kind1 naming rules instead of the $kind2 rules." if $kind2;
$o .= "<li><b>Minimum Length:</b> Specify the shortest length for a valid name. Prefixes and Suffixes are included in the count. The Default value is 1 character.\n";
$o .= "<li><b>Maximum Length:</b> Specify the longest length for a valid name. Prefixes and Suffixes are included in the count. The Default value is 0, which means there is no max length.\n";
$o .= "<li><b>Prefix and Suffix:</b> If the entity needs to have a prefix or suffix, specify them here. If there are multiple valid options, separate them with a coma. If the entity should not have a particular prefix or suffix, put an exclamation (!) before the string.\n";
$o .= "<li><b>Character Set:</b> Limit valid characters to Letters only (a-zA-Z), letters and numbers (a-zA-Z0-9) or All Characters (default). Prefixes and suffixes are ignored for this test.\n";
$o .= "<li><b>Capitalization:</b> The default option is to ignore capitalization. CamelCase allows upper and lower case letters but must start with a lower case letter for lowerCamelCase or a capital letter for UpperCamelCase. Alternatively you can require all capital letters or no capital letters.  By default there can be two Capital Letters next to each other, but that can be disallowed. Required prefixes and suffixes are not tested for capitalization.\n";
$o .= "<li><b>Regular Expression:</b> For complete flexibility you can test the name against a perl regular expression. To find results that do not match the expression put an exclamation (!) before it.\n";
$o .= "</ul>";
return $o;
}


sub setNamingOptions{
  my $check = shift;

  if ($defaultAll){
    setindividualOptions($check,$kind,scalar(@kindStrings)+1,1);
  }
  
  my $precCount = 1;
  foreach my $itemKind (@kindStrings){   
    setindividualOptions($check,$itemKind,$precCount);
    $precCount++;
  }

}

sub setindividualOptions{
  my ($check, $itemKind,$precCount, $testAll) = @_;
  my $name;
  my $precedenceTotal = scalar @kindStrings;
  if ($testAll && $defaultAll){
    $name = "All $kind";
  }else{
    $name = $kindNameLookup{$itemKind};
  }
  if ($defaultAll){
    $precedenceTotal++;
  }
  my $runValue = $precedenceTotal>1?0:1;
  $runValue = 1 if $testAll;
  my $space = "          ";

  $check->option->checkbox($itemKind."Run", "Test ${name} names",$runValue);
  $check->option->choice($itemKind."Precedence",$space."Precedence",[1..$precedenceTotal],$precCount) if $precedenceTotal>1  && $canChangePrecedence;
  $check->option->integer($itemKind."minLength", $space."Minimum Length:",1);
  $check->option->integer($itemKind."maxLength", $space."Maximum Length:");
  $check->option->text($itemKind."prefix", $space."Required Prefix:");
  $check->option->text($itemKind."suffix", $space."Required Suffix:");
  $check->option->choice($itemKind."charSet", $space."Character Set",["All Characters","Letters and Numbers Only","Letters Only"],"All Characters");
  $check->option->choice( $itemKind."capOpt", $space."Capitalization:",["ignore","UpperCamelCase","lowerCamelCase","ALLCAPS","nocaps"],"ignore");
  $check->option->choice($itemKind."consCaps", $space."Consecutive Capitals",["Allowed","Not Allowed"],"Allowed");
  $check->option->text($itemKind."regExp", $space."Regular Expression:");
}

sub checkNamingRules{
  my ($name,$type,$check) = @_;

  my $errors;
  my $len = length($name);
  my $minLength = $check->option->lookup($type."minLength");
  my $maxLength = $check->option->lookup($type."maxLength");
  my @prefixList = split(/,/,$check->option->lookup($type."prefix"));
  my $regExp = $check->option->lookup($type."regExp");
  my @suffixList = split(/,/,$check->option->lookup($type."suffix"));
  my $capOpt = $check->option->lookup($type."capOpt");
  my $charSet = $check->option->lookup($type."charSet");
  my $allowConsCaps = $check->option->lookup($type."consCaps") =~ /^Allowed$/;
  my $runTest = $check->option->lookup($type."Run");
  my $subName = $name;
  
  return unless $runTest;
  
  if($maxLength>0 && $len > $maxLength){
    $errors = TOOLONG;
  }
  
  if($len < $minLength){
    $errors .= TOOSHORT;
  }
  
  if ($regExp){
    my $regExpUsed = 0;
    my $badRegExp = 0;
    $regExp =~ s/^\s+|\s+$//; 
    if ($regExp =~ s/^[!]//){   
      $badRegExp=1;
    }
    if ($subName =~ /$regExp/){
      $regExpUsed =1;
      $subName = $1;
    }
    if ($regExpUsed && $badRegExp){
      $errors .= BADREGEXP;
    }
    if (!$regExpUsed && ! $badRegExp){
      $errors .= MISSINGREGEXP;
    }
  }
  
  #Process Prefix and Suffix, don't use for the rest of the tests  
  if (@prefixList || @suffixList){
    my $prefixUsed = 0;
    my $badPrefix = 0;
    PREFIX:foreach my $prefix( @prefixList){
      $prefix =~ s/^\s+|\s+$//; 
      if ($prefix =~ s/^[!]//){   
        $badPrefix=1;
      }
      if ($subName =~ /^$prefix(.*)/){
        $prefixUsed =1;
        $subName = $1;
        last PREFIX;
      }
    }
    if ($prefixUsed && $badPrefix){
      $errors .= BADPREFIX;
    }
    if (@prefixList && !$prefixUsed && ! $badPrefix){
      $errors .= MISSINGPREFIX;
    }
    my $suffixUsed = 0;
    my $badSuffix = 0;
    SUFFIX:foreach my $suffix( @suffixList){
      $suffix =~ s/^\s+|\s+$//; 
      if ($suffix =~ s/^[!]//){
        $badSuffix=1;
      }
      if ($subName =~ /(.*)$suffix$/){
        $suffixUsed =1;
        $subName = $1;
        last SUFFIX;
      }
    }
    if ($suffixUsed && $badSuffix){
      $errors .= BADSUFFIX;
    }
    if (@suffixList && !$suffixUsed && ! $badSuffix){
      $errors .= MISSINGSUFFIX;
    }
  }
  
  #Check Case
  if ($capOpt =~ /UpperCamelCase/ && $subName !~ /^[A-Z]/){
    $errors .= MISSINGINITCAP;
  }
  if ($capOpt =~ /lowerCamelCase/ && $subName !~ /^[a-z]/){
    $errors .= MISSINGINITLC;
  }
  if ($capOpt =~ /ALLCAPS/ && $subName =~ /[a-z]/){
    $errors .= HASLOWERCASE;
  }
  if ($capOpt =~ /nocaps/ && $subName =~ /[A-Z]/){
    $errors .= HASUPPERCASE;
  }
  if(! $allowConsCaps && $subName =~ /[A-Z][A-Z]/){
    $errors .= CONSECCAPS;
  }

  #Check character set
  if($charSet =~ /Letters and Numbers Only/ && $subName !~ /^[A-Za-z0-9]+$/){
    $errors .= NONALPHANUM;
  }
  if($charSet =~ /Letters Only/ && $subName !~ /^[A-Za-z]+$/){
    $errors .= NONALPHA;
  }
  
  return $errors;
}

sub getPrimaryKind{
  my $check = shift;
  my $kindList = shift;
  
    my @varKinds;
    #Discard invalid kinds (those not in the testing list)
    foreach my $kind (split(/ /,$kindList)){
      push(@varKinds, $kind) if $check->option->lookup($kind."Run");
    }
    
    #Sort the valid kind list based off the precedence option and get the first kind
    @varKinds = sort{$check->option->lookup($a."Precedence") <=> $check->option->lookup($b."Precedence");} @varKinds;
    return $varKinds[0] if @varKinds;
    
    return $kind if $defaultAll;
    
}



1;