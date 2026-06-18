/* C++ examples for MISRA C 2012 / 2023 Rule 1.2 and MISRA C 2025 Directive 1.2.
   Confirms that extension keyword detection works in C++ files and that the
   C++-specific paths (no statement-expression detection, standard attributes
   and keywords left alone) behave as documented. */

#include <cstddef>

/* Extension keywords are flagged the same way in C++ as in C */
static int gnu_unused __attribute__((unused));   // UndCC_Violation
int *__restrict rp;                              // UndCC_Violation
__typeof__(int) tv;                              // UndCC_Violation

int likely_true(int c)
{
  return __builtin_expect(c, 1);                 // UndCC_Violation
}

void barrier()
{
  __asm__ volatile ("" ::: "memory");            // UndCC_Violation
}

/* GNU __alignof__ is an extension; the standard alignof keyword is not */
unsigned a1 = __alignof__(int);                  // UndCC_Violation
unsigned a2 = alignof(int);                      // UndCC_Valid - standard keyword

/* A braced-init-list argument produces a '(' immediately followed by '{'. This
   is the pattern that statement-expression detection would flag, so that
   detection is disabled for C++ to avoid this false positive. */
struct Point { int x; int y; };
void take_point(Point p);

void use_braced()
{
  take_point({1, 2});                            // UndCC_Valid - braced-init-list, not a statement expression
}

/* GNU statement expressions are a known C++ limitation: they are not detected
   because they are indistinguishable from braced-init-list arguments. */
int stmt_expr()
{
  return ({ int t = 1; t; });                    // UndCC_Valid - statement expressions are not detected in C++
}

/* Standard C++ constructs must NOT be flagged */
[[nodiscard]] int compute();                     // UndCC_Valid - standard attribute
void may_throw() noexcept;                        // UndCC_Valid - standard keyword

const char *who()
{
  return __func__;                               // UndCC_Valid - standard predefined identifier
}

/* Microsoft extensions are flagged even in an inactive block (the lexer still
   scans the tokens) - same behaviour as the C test. */
#if defined(_WIN32)
__declspec(dllimport) void imported();           // UndCC_Violation
__forceinline int fast_add(int a, int b) { return a + b; }  // UndCC_Violation
#endif

/* Ordinary compliant C++ code */
int add(int a, int b)
{
  int sum = a + b;                               // UndCC_Valid
  return sum;
}
