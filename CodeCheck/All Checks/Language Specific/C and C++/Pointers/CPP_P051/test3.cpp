// Custom

#include <stdint.h>

void f(void)
{
}

void func(unsigned int flag)
{
  char *ptr;
  uintptr_t number0 = (uintptr_t) ptr; // UndCC_Violation

  char *ptr1;
  unsigned int number = (intptr_t) ptr1; // UndCC_Violation

  number = (number & 0x7fffff) | (flag << 23);
  ptr1 = (char *) number;
  int *p;
  int addr = (intptr_t) &p; // UndCC_Violation
}
