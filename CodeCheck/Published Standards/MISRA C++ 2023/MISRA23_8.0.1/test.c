/* MISRA C 2012 Rule 12.1 Test File
 * The precedence of operators within expressions should be made explicit
 *
 * Test cases based on examples from the MISRA C 2012 specification
 */

/* Unary and postfix operators with high-precedence operands */
struct S { int n; };

void test_unary_postfix(void)
{
  struct S a[10];
  struct S *p = 0;
  int x = 0;
  int y = 0;
  int i = 0;

  a[ i ].n;       /* UndCC_Valid - no need to write ( a[ i ] ).n  */
  *p++;           /* UndCC_Valid - no need to write *( p++ )      */
  sizeof x + y;   /* UndCC_Violation - write either sizeof ( x ) + y or sizeof ( x + y ) */
}

/* Same-precedence operators */
void test_same_precedence(void)
{
  int a = 1, b = 2, c = 3, d = 4;

  a + b;                    /* UndCC_Valid */
  a + b + c;                /* UndCC_Valid */
  ( a + b ) + c;            /* UndCC_Valid */
  a + ( b + c );            /* UndCC_Valid */
  a + b - c + d;            /* UndCC_Valid */
  ( a + b ) - ( c + d );    /* UndCC_Valid */
}

/* Function calls with expression arguments */
int f(int x, int y);

void test_function_calls(void)
{
  int a = 1, b = 2, c = 3;
  int x;

  x = f ( a + b, c );       /* UndCC_Valid - no need to write f ( ( a + b ), c ) */
}

/* Conditional operator with mixed precedences */
void test_conditional_operator(void)
{
  int a = 1, b = 2;
  int x;

  /* Operands of conditional operator (precedence 2) need parentheses
   * around == (precedence 8) and - (precedence 11) */
  x = a == b ? a : a - b;       /* UndCC_Violation */
  x = ( a == b ) ? a : ( a - b ); /* UndCC_Valid */
}

/* Shift operator with additive operand */
void test_shift_operator(void)
{
  int a = 1, b = 2, c = 3;
  int x;

  /* << (precedence 10) with + (precedence 11) operand */
  x = a << b + c;               /* UndCC_Violation */
  x = a << ( b + c );           /* UndCC_Valid */
}

/* Logical operators */
void test_logical_operators(void)
{
  int a = 1, b = 1, c = 1;

  if ( a && b && c ) { }        /* UndCC_Valid - all operators are the same */
  if ( a && b || c ) { }        /* UndCC_Violation - mixed && and || */
  if ( a || b && c ) { }        /* UndCC_Violation - mixed || and && */
  if ( a || b || c ) { }        /* UndCC_Valid - all operators are the same */
  if ( ( a && b ) || c ) { }    /* UndCC_Valid - properly parenthesized */
  if ( a || ( b && c ) ) { }    /* UndCC_Valid - properly parenthesized */
}

/* Bitwise operators */
void test_bitwise_operators(void)
{
  int a = 1, b = 2, c = 3;
  int x;

  x = a & b & c;                /* UndCC_Valid - same operators */
  x = a & b | c;                /* UndCC_Violation - mixed & and | */
  x = a | b & c;                /* UndCC_Violation - mixed | and & */
  x = ( a & b ) | c;            /* UndCC_Valid */
  x = a | ( b & c );            /* UndCC_Valid */
}

/* Assignment is precedence 1 - operands do not need parentheses */
void test_assignment(void)
{
  int a = 1, b = 2;
  int x;

  x = a + b;                    /* UndCC_Valid - assignment has precedence 1 */
  x = a == b ? a : b;           /* UndCC_Violation - ternary operands need parentheses */
}

/* Relational with equality */
void test_relational_equality(void)
{
  int a = 1, b = 2, c = 3, d = 4;
  int result;

  result = a < b == c < d;      /* UndCC_Violation - relational operands need parentheses around == */
  result = ( a < b ) == ( c < d ); /* UndCC_Valid */
}

/* Bitwise AND with equality */
void test_bitwise_equality(void)
{
  int a = 1, b = 2, c = 3;
  int result;

  result = a & b == c;          /* UndCC_Violation - & needs parentheses around == */
  result = ( a & b ) == c;      /* UndCC_Valid */
}

/* Complex nested expressions */
void test_complex_nested(void)
{
  int a = 1, b = 2, c = 3, d = 4;
  int x;

  x = a + b * c - d;            /* UndCC_Violation - * operand needs parentheses */
  x = a * b + c * d;            /* UndCC_Violation - * operands need parentheses */
  x = a + ( b * c ) - d;        /* UndCC_Valid */
  x = ( a * b ) + ( c * d );    /* UndCC_Valid */
  x = ( a + b ) * ( c - d );    /* UndCC_Valid */
}

/* Comma operator: this rule does not require comma operands to be parenthesized.
 * Use of the comma operator is prohibited by Rule 12.3, not this rule. */
void test_comma_operator(void)
{
  int a = 1, b = 2;
  int x;

  x = a, b;                     /* UndCC_Valid - parsed as ( x = a ), b */
}

/* Expressions inside macro expansions — users cannot add parentheses inside
 * a macro they don't control. */
#define MAKELONG(low, high) \
  ((unsigned long)(((unsigned short)(low)) | ((unsigned long)((unsigned short)(high))) << 16))
#define MAKELPARAM(l, h)  ((long)MAKELONG(l, h))

void test_macro_expansions(void)
{
  int cursorrow = 0, cursorcol = 0;
  long lParam;

  lParam = MAKELPARAM(cursorrow, cursorcol); /* UndCC_Valid - operators are inside macro */
}

/* Note: Preprocessor conditionals (#if defined(X) && ...) from the MISRA
 * examples are not analyzed by Clang AST checkers since they are processed
 * before the AST is built. */
