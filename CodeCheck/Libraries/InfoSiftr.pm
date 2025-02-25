# a set of Codecheck helper functions developed by InfoSiftr
package CodeCheck::Libraries::InfoSiftr;
use base qw(Exporter);

use strict;

our @EXPORT_OK = qw(lexemeIsWhitespace skipWhitespace resolveTypedefs getNextFunctionParam getFunctionParams getObjectBitFieldWidth);

sub lexemeIsWhitespace {
	my $lexeme = shift;
	
	return $lexeme && (
		$lexeme->token eq 'Whitespace'
		|| $lexeme->token eq 'Comment'
		|| $lexeme->token eq 'Newline'
	);
}

sub skipWhitespace {
	my $lexeme = shift;
	
	while (lexemeIsWhitespace($lexeme)) {
		$lexeme = $lexeme->next;
	}
	
	return $lexeme;
}


sub resolveTypedefs {
	my ($type, $db,@tracking)=@_;
	
	return $type if !defined $type || $type eq '';
	return '' if grep {$_ eq $type} @tracking;
	push(@tracking,$type);
	
	my $changed;
	do {
		return $type if $type =~ m!\b( struct | union )\b!x; # so that "typedef struct name { ... } name;" doesn't infinite loop us
		
		$changed = 0;
		my @words = split m/[\s*?]+/, $type; # we include * and ? here because they're special characters according to "lookup"
		
		for my $word (@words) {
			if (my @ents = $db->lookup($word, 'typedef',1)) {
				my $ent = $ents[0];
				my $newType = resolveTypedefs($ent->type, $db,@tracking);
				$type =~ s/\b \Q$word\E \b/$newType/xg;
				$changed = 1;
				
			}
		}
	} while ($changed);
	
	return $type;
}

sub getNextFunctionParam {
	my $lexeme = shift; # should represent either "functionName", "(", or ","
	my $rawData = do { @_ ? shift : 0 };
	
	$lexeme = skipWhitespace($lexeme);
	
	if (
		$lexeme
		&& $lexeme->text ne '('
		&& $lexeme->text ne ','
	) {
		# must be "functionName", so skip it and move on to "("
		until (!$lexeme || $lexeme->text eq '(' || $lexeme->text eq ',') {
			$lexeme = $lexeme->next;
			$lexeme = skipWhitespace($lexeme);
		}
		# This is a special case for the operator() overload
		if ( $lexeme and $lexeme->prevUseful and $lexeme->nextUseful and ( lc( $lexeme->prevUseful->text ) eq 'operator' ) and ( $lexeme->nextUseful->text eq ')' ) ) {
			$lexeme = $lexeme->nextUseful;
			$lexeme = $lexeme->nextUseful;
		}
	}
	
	return (undef, undef) unless $lexeme;
	
	my $param = '';
	my @data;
	while ($lexeme->next && $lexeme->next->text !~ m! ^ (?: , | \) ) $ !x) {
		$lexeme = $lexeme->next;


		# If we encounter std::array< T, T > or std::vector< T > we need to skip as these are type arguments to the standard library functions
		# We need to move from the opening < to the final closing > to correctly parse this parameter list
		if ( $lexeme and $lexeme->token eq 'Operator' and $lexeme->text eq '<' ) {

			my $bracketCount = 1;
			$lexeme = $lexeme->next;

			while( $lexeme and $bracketCount > 0 ) {
				$bracketCount++ if $lexeme->token eq 'Operator' and $lexeme->text eq '<';
				$bracketCount-- if $lexeme->token eq 'Operator' and $lexeme->text eq '>';
				$lexeme = $lexeme->next;
			}
		}
		return unless $lexeme;
		# End special case for std::array and std::vector

		
		if (lexemeIsWhitespace($lexeme)) {
			$param .= ' ';
		}
		else {
			$param .= $lexeme->text;
			
			push @data, {
				text => $lexeme->text,
				token => $lexeme->token,
				ent => $lexeme->ent,
			};
		}
	}
	
	$param =~ s! \s{2,} ! !gx;
	$param =~ s! ^ \s !!gx;
	$param =~ s! \s $ !!gx;
	
	return (($rawData ? \@data : $param), $lexeme->next || undef);
}

sub getFunctionParams {
	my $ref = shift;
	my $lexer = shift;
	my $rawData = do { @_ ? shift : 0 };
	
	return () unless $lexer;
	
	return () unless $ref->ent->kind->check('function');
	return () unless $ref->kind->check('define, declare');
	
	my $lexeme = $lexer->lexeme($ref->line, $ref->column);
	
	return () unless $lexeme;
	
	my @params;
	while ($lexeme && $lexeme->text ne ')') {
		(my $param, $lexeme) = getNextFunctionParam($lexeme, $rawData);
		
		push @params, $param if defined $param && ($rawData || $param ne '');
	}
	
	return @params;
}

sub getObjectBitFieldWidth {
	my $ent = shift;
	my $lexer = shift;
	
	return undef unless $lexer;
	
	return undef unless $ent->kind->check('object'); # only Objects may be bit fields
	
	my $lexeme = $lexer->lexeme($ent->ref->line, $ent->ref->column);
	
	return undef unless $lexeme;
	
	$lexeme = skipWhitespace($lexeme->next);
	
	return undef unless $lexeme && $lexeme->text eq ':'; # hey, this looks like a bit field! :)
	
	$lexeme = skipWhitespace($lexeme->next);
	
	return undef unless $lexeme && $lexeme->text =~ m/ ^ \d+ $ /x;
	
	return int $lexeme->text;
}

1;
