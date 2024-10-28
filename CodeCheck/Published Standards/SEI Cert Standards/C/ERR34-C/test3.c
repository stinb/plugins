// Custom

#include <stdlib.h>

void func(const char *buff) {
  int si;

  if (buff) {
    si = atoi(buff);  // UndCC_Violation
    si = atol(buff);  // UndCC_Violation
    si = atoll(buff); // UndCC_Violation
    si = atof(buff);  // UndCC_Violation
  }
  else {
    /* Handle error */
  }
}
