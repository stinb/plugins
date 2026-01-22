// MISRA C++ 2023 Rule 8.0.1 Test File
// Parentheses should be used to make the meaning of an expression appropriately explicit
//
// Test cases based on examples from the MISRA C++ 2023 specification

#include <cstdint>

// Test: Unary and postfix operators with high-ranking operands
struct S { int n; };

void test_unary_postfix()
{
  struct S a[10];
  struct S *p = nullptr;
  int i = 0;
  int x = 0;
  int y = 0;

  a[i].n;               // UndCC_Valid - no need extra parentheses
  *p++;                 // UndCC_Valid - no need to write *( p++ )
  sizeof x + y;         // UndCC_Violation - write either sizeof ( x ) + y or sizeof ( x + y )
}

// Test: Same-ranking operators
void test_same_ranking()
{
  int a = 1, b = 2, c = 3, d = 4;

  a + b - c + d;        // UndCC_Valid
  (a + b) - (c + d);    // UndCC_Valid - produces a different result
}

// Test: Function calls with expressions
int f(int x, int y);

void test_function_calls()
{
  int a = 1, b = 2, c = 3;
  int x;

  x = f(a + b, c);      // UndCC_Valid - no need to write f ( ( a + b ), c )
}

// Test: Conditional operator with mixed rankings
void test_conditional_operator()
{
  int a = 1, b = 2;
  int x;

  x = a == b ? a : a - b;       // UndCC_Violation - operands of conditional operator need parentheses
  x = (a == b) ? a : (a - b);   // UndCC_Valid - compliant version
}

// Test: Shift operator with additive operand
void test_shift_operator()
{
  int a = 1, b = 2, c = 3;
  int x;

  x = a << b + c;               // UndCC_Violation - + (ranking 12) needs parentheses
  x = a << (b + c);             // UndCC_Valid - compliant version
}

// Test: Logical operators
void test_logical_operators()
{
  bool a = true, b = true, c = true;

  a && b && c;          // UndCC_Valid - all operators are the same
  a && b || c;          // UndCC_Violation - mixed && and ||
  a || b && c;          // UndCC_Violation - mixed || and &&
  a || b || c;          // UndCC_Valid - all operators are the same
  (a && b) || c;        // UndCC_Valid - properly parenthesized
  a || (b && c);        // UndCC_Valid - properly parenthesized
}

// Test: Bitwise operators
void test_bitwise_operators()
{
  int a = 1, b = 2, c = 3;

  a & b & c;            // UndCC_Valid - same operators
  a & b | c;            // UndCC_Violation - mixed & and |
  a | b & c;            // UndCC_Violation - mixed | and &
  (a & b) | c;          // UndCC_Valid - properly parenthesized
  a | (b & c);          // UndCC_Valid - properly parenthesized
}

// Test: Assignment operators (ranking 2 - should not require operand parentheses)
void test_assignment()
{
  int a = 1, b = 2, c = 3;
  int x;

  x = a + b;            // UndCC_Valid - assignment has ranking 2
  x = a == b ? a : b;   // UndCC_Violation - ternary operands need parentheses
}

// Test: Relational with equality
void test_relational_equality()
{
  int a = 1, b = 2, c = 3, d = 4;
  bool result;

  result = a < b == c < d;      // UndCC_Violation - relational operators need parentheses around equality
  result = (a < b) == (c < d);  // UndCC_Valid
}

// Test: Bitwise AND with equality
void test_bitwise_equality()
{
  int a = 1, b = 2, c = 3;
  bool result;

  result = a & b == c;          // UndCC_Violation - & needs parentheses around ==
  result = (a & b) == c;        // UndCC_Valid
}

// Test: Complex nested expression
void test_complex_nested()
{
  int a = 1, b = 2, c = 3, d = 4;
  int x;

  x = a + b * c - d;            // UndCC_Violation - * operand needs parentheses
  x = a * b + c * d;            // UndCC_Violation - * operands need parentheses
  x = a + (b * c) - d;          // UndCC_Valid - multiplicative expression properly parenthesized
  x = (a * b) + (c * d);        // UndCC_Valid - multiplicative expressions properly parenthesized
  x = (a + b) * (c - d);        // UndCC_Valid - explicit precedence override
}

// Note: Preprocessor conditionals (#if, etc.) are not analyzed by Clang AST checkers
// since they are processed before the AST is built
