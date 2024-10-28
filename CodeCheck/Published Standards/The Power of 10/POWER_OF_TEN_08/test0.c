#include <stdio.h>

#define A 0
#define B 1

#if A                                           // UndCC_Violation - too many conditionals
  #define X 1
#elif B                                         // UndCC_Violation - too many conditionals
  #define X 1
#endif

#undef B                                        // UndCC_Violation - directive not allowed
#include_next "stdlib.h"                        // UndCC_Violation - directive not allowed
#warning "Hello there"                          // UndCC_Violation - directive not allowed

#define DEBUG(x, ...) printf(x, __VA_ARGS__)    // UndCC_Violation - variadic arguments

#define STRINGIZE(n) printf_s("token"#n)        // UndCC_Violation - operator not allowed
#define PASTE(n) printf_s(token##n)             // UndCC_Violation - operator not allowed

#define MIN(x, y) ((y < x) ? y : x)
#define MACRO_IN_DEF_FUNCTIONAL(x, y) MIN(x, y) // UndCC_Violation(1) - not allowed by option
#define MACRO_IN_DEF_LITERAL A                  // UndCC_Violation(1) - not allowed by option

#define BRACES(x, y) {}                         // UndCC_Violation(1) - not allowed by option
#define MULTIPLE_LINES 1                        /* UndCC_Violation(1) - not allowed by option */ \
        + 1
