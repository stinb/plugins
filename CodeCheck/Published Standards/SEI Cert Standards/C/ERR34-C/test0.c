// SEI CERT C

#include <stdlib.h>

void func(const char *buff) {
  int si;

  if (buff) {
    si = atoi(buff); // UndCC_Violation
  } else {
    /* Handle error */
  }
}
