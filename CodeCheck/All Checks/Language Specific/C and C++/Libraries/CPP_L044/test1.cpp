// Custom

#include <stdio.h>

void printf(void);

void f()
{
  void (*some_printf)(void) = printf; // UndCC_Violation
}
