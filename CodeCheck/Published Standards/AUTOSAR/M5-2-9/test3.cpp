// Custom

#include <stdint.h>

void f(void)
{
}

void func(unsigned int flag)
{

  char *ptr;
  uintptr_t number0 = (uintptr_t) ptr; // UndCC_Violation(LIN,WIN)

  char *ptr1;
#if defined(_WIN32) || defined(__APPLE__)
  unsigned int number = (unsigned int) ptr1; // UndCC_Violation(WIN)
  number = (number & 0x7fffff) | (flag << 23);
  ptr1 = (char *) number;

  int *p;
  int addr = (int) &p; // UndCC_Violation(WIN)
#endif
}
