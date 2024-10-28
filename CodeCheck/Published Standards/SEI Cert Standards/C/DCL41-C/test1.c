#include <stdio.h>

extern void f(int i);

int func(int expr) {
  /*
   * Move the code outside the switch block; now the statements
   * will get executed.
   */
  int i = 4;
  f(i);

  switch (expr) {
    case 0:
      i = 17;
      /* Falls through into default code */
    default:
      printf("%d\n", i);
  }
  return 0;
}
