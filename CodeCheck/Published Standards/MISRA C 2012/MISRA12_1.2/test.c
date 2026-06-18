/* Examples for MISRA C 2012 / 2023 Rule 1.2 and MISRA C 2025 Directive 1.2 -
   "Language extensions should not be used / should be minimized". */

#include <stddef.h>

/* GNU type and storage-class extensions */
__extension__ typedef int gnu_int;          // UndCC_Violation
static __thread int tls_counter;             // UndCC_Violation
__int128 wide_value;                         // UndCC_Violation

/* GNU __attribute__ extension */
void deprecated_fn(void) __attribute__((deprecated));  // UndCC_Violation

/* GNU __typeof__ / __typeof extension */
#define SWAP(a, b) do { __typeof__(a) t = (a); (a) = (b); (b) = t; } while (0)  // UndCC_Violation

/* GNU qualifier spellings */
int *__restrict rp;                          // UndCC_Violation
volatile int vi;                             // UndCC_Valid - standard keyword

/* GNU __builtin_ intrinsics (prefix match) */
int likely_true(int c)
{
  return __builtin_expect(c, 1);             // UndCC_Violation
}

/* GNU inline assembly extension */
void barrier(void)
{
  __asm__ volatile ("" ::: "memory");        // UndCC_Violation
}

/* GNU statement expression (C only) */
#define MAX(a, b) ({ int _a = (a); int _b = (b); _a > _b ? _a : _b; })  // UndCC_Violation

int use_max(int x, int y)
{
  return MAX(x, y);                          // UndCC_Valid - expansion site, not the operator
}

/* Microsoft extensions. Wrapped in an inactive block so the non-Windows
   analyzer does not raise parse errors; the lexer still scans the tokens. */
#if defined(_WIN32)
__declspec(dllimport) void imported(void);   // UndCC_Violation
__forceinline int fast_add(int a, int b) { return a + b; }  // UndCC_Violation
#endif

/* Standard reserved identifiers must NOT be flagged */
const char *here = __FILE__;                 // UndCC_Valid
int current_line = __LINE__;                 // UndCC_Valid

const char *who(void)
{
  return __func__;                           // UndCC_Valid
}

/* Predefined feature macros are not extension keywords */
#ifdef __GNUC__                              // UndCC_Valid
int compiled_with_gcc = 1;                   // UndCC_Valid
#endif

/* Ordinary compliant code */
int add(int a, int b)
{
  int sum = a + b;                           // UndCC_Valid
  return sum;
}

/* A standard parenthesized expression is not a statement expression */
int grouped(int x)
{
  return (x + 1) * 2;                        // UndCC_Valid
}
