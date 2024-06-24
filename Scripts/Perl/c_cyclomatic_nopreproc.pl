# Synopsis: Calculate cyclomatic complexity without preprocessor substitutions.
#
# Categories: Project Metrics
#
# Languages: C
#
# Usage: uperl c_cyclomatic_nopreproc.pl -db path_to_database [options]
#        -db               (required) Path to the understand database
#        -long-file        (optional) Print long file names
#        -long-function    (optional) Print long function names
#      
#
# Description:       
#   This script calculates the cyclomatic complexity before
#   any preprocessor substitutions are made.  Thus, results
#   will likely vary from those produced by understand.
#


use strict;
use Understand;
use Getopt::Long;

my ($arg_db, $arg_long_file, $arg_long_func);
GetOptions("db=s"          => \$arg_db,
           "long-file"     => \$arg_long_file,
           "long-function" => \$arg_long_func);

my $db;
if ($arg_db && -e $arg_db) {
    $db = Understand::open($arg_db);
} else {
    $db = Understand::Gui::db();
}
die usage() unless $db;

print_cyclomatic_complexities($db);

sub print_cyclomatic_complexities ($) {
    my $db = shift;
    print "file, function, regular, modified, strict\n";
    foreach my $function ($db->ents("Function ~Unknown ~Unresolved")) {
        my $def_ref = $function->ref("Definein");
        if($def_ref) {
            my $complexity = cyclomatic_complexity($function);
            my ($file_name, $func_name);
            if ($arg_long_file) {
                $file_name = $def_ref->file()->longname();
            } else {
                $file_name = $def_ref->file()->name();
            }
            if ($arg_long_func) {
                $func_name = $function->longname();
            } else {
                $func_name = $function->name();
            }
            printf "%s, %s, %s, %s, %s\n",
                $file_name . ":" . $def_ref->line(),
                $func_name, regular_cc($complexity),
                modified_cc($complexity), strict_cc($complexity);
        } else {
            printf "Warning! Unable to find definition for %s.\n",
                $function->name();
        }
    }
}

sub regular_cc {
    my $hash = shift;
    my @controls = ("case", "for", "if", "?", "while");
    my $complexity;
    foreach my $control (@controls) {
        $complexity += $hash->{$control};
    }
    return 1 + $complexity;
}

sub modified_cc {
    my $hash = shift;
    my @controls = ("switch", "for", "if", "?", "while");
    my $complexity;
    foreach my $control (@controls) {
        $complexity += $hash->{$control};
    }
    return 1 + $complexity;
}

sub strict_cc {
    my $hash = shift;
    my @controls = ("&&", "||", "case", "for", "if", "?", "while");
    my $complexity;
    foreach my $control (@controls) {
        $complexity += $hash->{$control};
    }
    return 1 + $complexity;
}

sub cyclomatic_complexity {
    my $function = shift;
    
    my $def_ref = $function->ref("Definein"); # beginning of function
    my $end_ref = $function->ref("End");      # end of function
    my $lexer = $def_ref->file()->lexer();
    my @lexemes = $lexer->lexemes($def_ref->line(), $end_ref->line());
    
    my %hash;
    while (my $lexeme = shift @lexemes) {
    	$hash{$lexeme->text()}++;
    }
    return \%hash;
}

sub usage {
return << "END";
Usage: uperl c_cyclomatic_nopreproc.pl -db path_to_database [options]
       -db               (required) Path to the understand database
       -long-file        (optional) Print long file names
       -long-function    (optional) Print long function names
       
       This script calculates the cyclomatic complexity before
       any preprocessor substitutions are made.  Thus, results
       will likely vary from those produced by understand.
END
}