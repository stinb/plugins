#include <stdio.h>
#include <string.h>

void testFunc(void) {

  char *p = "hello world";
  p[0] = 'H'; /* UNDCC_Violation - undefined behavior */

  char a[] = "hello, world";
  char *p2 = a;  /* UndCC_Valid */

  a[0] = 'H';
  p[7] = 'W';  /* UNDCC_Violation - undefined behavior */

}
