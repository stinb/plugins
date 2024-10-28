#include <stdio.h>

#ifndef MESSAGE // UndCC_Violation - conditional compilation directive
#define MESSAGE "Error"
#endif

#define rec(n) ((n == 1) ? 1 : rec(n - 1)) // UndCC_Violation(1) - recursive macro

#define printnum(n) printf_s("nul" #n " = %d", num##n) // UndCC_Violation - token pasting

void foo(int x, ...) // UndCC_Violation(1) - using variable argument lists (ellipses)
{
}
