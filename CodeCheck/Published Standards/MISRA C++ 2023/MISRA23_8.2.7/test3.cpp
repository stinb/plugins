// Custom

#include <stdint.h>

void f(void)
{
}

void func(unsigned int flag)
{

  char *ptr;
  uintptr_t number0 = (uintptr_t) ptr; // UndCC_Violation
#if defined(_WIN32) || defined(__APPLE__)
  char *ptr1;
  intptr_t number = (intptr_t) ptr1; // UndCC_Violation(Win,Mac)
  number = (number & 0x7fffff) | (flag << 23);
  ptr1 = (char *) number;

  int *p;
  intptr_t addr = (intptr_t) &p; // UndCC_Violation(Win,Mac)
#endif
}
