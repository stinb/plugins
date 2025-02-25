#This library is designed for use in Understand - CodeChecks
#Kacy Stocks - Aug 21, 2018

# note: extern statement count is ignored
# Fuzzy Vs Strict Parser counting: sometimes they count statements differently.
# Public, private, and protected are counted differently between parsers.

package CodeCheck::Libraries::FileStatements;
use base qw(Exporter);

use strict;
use warnings;

use Exporter qw(import);
our @EXPORT_OK = qw(statements);

sub statements {
  my $file = shift;

  return unless $file->kind->check("file");             # check to make sure the file exists
  return unless my $lexer = $file->lexer(1, 8, 1, 1);   # sets defaults to lexer, except true on expanding macros
  
  my @statementlist;
  my @acclex;
  my @lexemes = $lexer->lexemes();
  my @statement;
  my $infuncdec = 0;
  my $inifdec = 0;
  my $parens = 0;
  my $classparens = 0;
  my $structparens = 0;
  my $eqdecparens = 0;
  my $infordec = 0;
  my $intypedef = 0;
  my $ineqdec = 0;
  my $inclassdec = 0;
  my $eqparens = 0;
  my $typedefparens = 0;
  my $whiledec = 0;
  my $instructdec = 0;
  my $prevlex;
  my @classdecstatement;
  my @structdecstatement;
  my @typedefstatement;

  MAIN:while (@lexemes){
    my $lex = shift(@lexemes);

    next if !@statement && $lex->token() eq "Whitespace";   # skips whitespace only lines
    next if $lex->inactive();  # skips inactive code
    next if $lex->token() eq "Comment"; # skips comments
    next if $lex->token() eq "Newline"; # skips newlines

    #skips preprocessors *** SHOULD BE THE ONLY OTHER WHILE LOOP!!! ***
    if ($lex->token() eq "Preprocessor"){
      while($lex->token() ne "Newline"){
        $lex = shift(@lexemes);
      }
    }

    # sets a boolean to true if this lex begins a signature
    if ( $lex->ref && $lex->ent && $lex->ref->kind->check("Definein") && $lex->ent->kind->check("Function")){
      $infuncdec = 1;
    }

    next unless $lex->text();

    if ($lex->text() eq "class" && $lex->token() eq "Keyword"){
      $inclassdec = 1;
    }

    # sets a boolean to true if this lex begins an if statement
    if ($lex->text() eq "if" && $lex->token() eq "Keyword"){
      $inifdec = 1;
    }

    # sets a boolean to true if this lex begins a for statement
    if ($lex->text() eq "for" && $lex->token() eq "Keyword"){
      $infordec = 1;
    }

    # sets a status variable to 1 if this lex begins a typedef statement
    if ($lex->text() eq "typedef" && $lex->token() eq "Keyword"){
      $intypedef = 1;
    }

    # sets a boolean to 1 if this lex begins a typedef statement
    if ($lex->text() eq "while" && $lex->token() eq "Keyword"){
      $whiledec = 1;
    }

    if ($lex->text() eq "=" && $lex->token() eq "Operator"){
      $ineqdec = 1;
    }

    if ($lex->text() eq "struct" && $lex->token() eq "Keyword"){
      $instructdec = 1;
    }

    # if assigning an ordered array with '{}' to a variable, don't terminate the statement at '{' or '}'
    if ($ineqdec != 0){
      if ($ineqdec == 1){
        if ($lex->text() ne "{" && $lex->text() ne ";"){
          push @statement, $lex;
          next MAIN;
        }
        if ($lex->text() eq ";"){                   # if lex is a ';' finish
          push @statement, $lex;
          push @statementlist, [@statement];
          @statement = ();
          $ineqdec = 0;
          next MAIN;
      }# For simple equal statements
        if ($lex->text() eq "{"){  # if it's complex, move on to ineqdec 2
          $ineqdec = 2;
        }
      }

      if ($ineqdec == 2){ # we've reached {
        push @statement, $lex;
        if ($lex->text() eq "{" && $lex->token() eq "Punctuation"){
          $eqdecparens++;
        }
        if ($lex->text() eq "}" && $lex->token() eq "Punctuation"){
          $eqdecparens--;
        }
        if ($eqdecparens == 0 && $lex->text() ne ";" && $lex->token() ne "Punctuation"){  # we've reached 0 parens, haven't reached ;
          next MAIN;
        }     #we've reached 0 parens, and ';' now finish the statement
        if ($lex->text() eq ";"){
          push @statementlist, [@statement];
          @statement = ();
          $ineqdec = 0;
        }
        next MAIN;
      }
    }
    
    #handles structs
    if ($instructdec != 0){
      if ($instructdec == 1){
        if ($lex->text() ne "{" && $lex->text() ne ";"){
          push @structdecstatement, $lex;
          next MAIN;
        }
        if ($lex->text() eq ";"){
          push @structdecstatement, $lex;
          push @statementlist, [@structdecstatement];
          @structdecstatement = ();
          $instructdec = 0;
        }
        push @structdecstatement, $lex;
        $instructdec = 2;
      }
      # struct declaration is finished, release to handle struct body
      if ($instructdec == 2){
        if ($lex->text() eq "{" && $lex->token() eq "Punctuation"){ # starts parens with 1
          $structparens++;
        }
        if ($lex->text() eq "}" && $lex->token() eq "Punctuation"){
          $structparens--;
        }
        if ($structparens == 0 && $lex->text() eq "}" && $lex->token() eq "Punctuation"){
          $instructdec = 3;
        }
      }
      # if parens is 0 and lex is '}' continue to instructdec 3
      if ($instructdec == 3){
        push @structdecstatement, $lex;
        if ($lex->text() eq ";" && $lex->token() eq "Punctuation"){
          push @statementlist, [@structdecstatement];
          @structdecstatement = ();
          $instructdec = 0;
        }
        next MAIN;
      }
    }

    # handles class def statements and allows the body to be released to be treated normally.
    if ($inclassdec != 0){
      if ($inclassdec == 1){
        if ($lex->text() ne "{" && $lex->token() ne "Punctuation"){
          push @classdecstatement, $lex;
          next MAIN;
        }
        push @classdecstatement, $lex;
        $inclassdec = 2;
      }
      # class declaration is finished, release to handle class body
      if ($inclassdec == 2){
        if ($lex->text() eq "{" && $lex->token() eq "Punctuation"){ # starts parens with 1
          $classparens++;
        }
        if ($lex->text() eq "}" && $lex->token() eq "Punctuation"){
          $classparens--;
        }
        if ($classparens == 0 && $lex->text() eq "}" && $lex->token() eq "Punctuation"){
          $inclassdec = 3;
        }
      }
      # if parens is 0 and lex is '}' continue to inclassdec 3
      if ($inclassdec == 3){
        push @classdecstatement, $lex;
        if ($lex->text() eq ";" && $lex->token() eq "Punctuation"){
          push @statementlist, [@classdecstatement];
          @classdecstatement = ();
          $inclassdec = 0;
        }
        next MAIN;
      }
    }

    # handles for statement terminations, as some do not end with a '{'
    if ($infordec == 1 && $lex->text() =~ /\(|\)/){
       if ($lex->text() eq "("){
        $parens++;
       }
       elsif ($lex->text() eq ")"){
        $parens--;
       }
       push @statement, $lex;
       if ($parens == 0){
        $infordec = 0;
        push @statementlist, [@statement];#
        @statement = ();
       }
       next MAIN;
    }

     # handles typedef statements ending in ";" with or without "{}"
    if ($intypedef != 0){
      if ($intypedef == 1 && $lex->text() ne "{" && $lex->text() ne ";"){
        push @typedefstatement, $lex;
        next MAIN;
      }
      if ($lex->text() eq ";"){                   # if lex is a ';' finish
        push @typedefstatement, $lex;
        push @statementlist, [@typedefstatement];
        @typedefstatement = ();
        $intypedef = 0;
        next MAIN;
      }# end of simple typedefs
      if ($intypedef == 1 && $lex->text() eq "{"){  # if it's complex, move on to typedef 2
        $intypedef = 2;
      }

      if ($intypedef == 2){ # we've reached {
        if ($lex->text() eq "{" && $lex->token() eq "Punctuation"){
          $typedefparens++;
        }
        if ($lex->text() eq "}" && $lex->token() eq "Punctuation"){
          $typedefparens--;
        }
        if ($typedefparens == 0 && $lex->text() ne ";" && $lex->token() ne "Punctuation"){  # we've reached 0 parens, haven't reached ;
          push @typedefstatement, $lex;
          next MAIN;
        }# we've reached 0 parens, and ';' now finish the statement
        push @statementlist, [@typedefstatement];
        @typedefstatement = ();
        $intypedef = 0;
        next MAIN;
        }
      }

    # public, private, and protected
    elsif (($lex->text() eq "public" || $lex->text() eq "private" || $lex->text() eq "protected") && $lex->token() eq "Keyword"){
      push @statement, $lex;
      $lex = $lex = shift(@lexemes);
      push @statement, $lex;
      @statement = ();
    }

    # handles signature of function terminations
    elsif ($infuncdec == 1 && $lex->text() ne "{" && $lex->token() ne "Punctuation"){
      push @statement, $lex;
    }
   
    # handles semi-colon terminated statements
    elsif ($lex->text() eq ";" && $lex->token() eq "Punctuation"){
      push @statement, $lex;
      push @statementlist, [@statement];#
      @statement = ();
    }

    # handles beginning-of-block terminated statements
    elsif ($lex->text() eq "{" && $lex->token() eq "Punctuation"){
      if (@statement){
        push @statement, $lex;
        push @statementlist, [@statement];#
        @statement = ();
        $infuncdec = 0;
      }
    }

    # handles end-of-block terminated statements
    elsif ($lex->text() eq "}" && $lex->token() eq "Punctuation" ){ 
      if(@statement){
        push @statement, $lex;
        push @statementlist, [@statement];#
        @statement = ();
      }
    }

    # handles if statement terminations, as some do not end with a '{'
    elsif ($inifdec == 1 && $lex->text() =~ /\(|\)/){
       if ($lex->text() eq "("){
        $parens++;
       }
       elsif ($lex->text() eq ")"){
        $parens--;
       }
       push @statement, $lex;
       if ($parens == 0){
        $inifdec = 0;
        push @statementlist, [@statement];#
        @statement = ();
       }
    }

    # handles while statement terminations, as some do not end with a '{'
    elsif ($whiledec == 1 && $lex->text() =~ /\(|\)/){
       if ($lex->text() eq "("){
        $parens++;
       }
       elsif ($lex->text() eq ")"){
        $parens--;
       }
       push @statement, $lex;
       if ($parens == 0){
        $whiledec = 0;
        push @statementlist, [@statement];#
        @statement = ();
       }
    }

    # handles tokens relevent to the current statement
    elsif ($lex->token() ne "Comment" && $lex->token() ne "Newline"){
      push @statement, $lex;
    }
  }# MAIN while loop
  return @statementlist;
}

1;