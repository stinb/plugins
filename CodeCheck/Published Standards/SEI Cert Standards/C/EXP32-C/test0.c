#include <stdio.h>

void func(void) {
  static volatile int **ipp;
  static int *ip;
  static volatile int i = 0;

  printf("i = %d.\n", i);

  ipp = &ip;         /* UndCC_Violation */
  ipp = (int**) &ip; /* UndCC_Violation */
  *ipp = &i;         /* UndCC_Valid */
  if (*ip != 0) {    /* UndCC_Valid */
    /* ... */
  }
}
