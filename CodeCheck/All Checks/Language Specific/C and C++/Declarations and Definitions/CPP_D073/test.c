#include <stdio.h>

// declare some globals
char a;
char b;
char c, d;                      /* UndCC_Valid - these variables are both one char long, do not flag */
int counter_1, testing_int;     /* UndCC_Violation - these are not essentially identical, flag */

int main() {
  
  // declare some locals
  int compound_num = 0;
  int mycount_1, mycount_2;
  
  char initial, marker, piece;  /* UndCC_Violation - these are not essentially identical, flag marker and piece */
  
  mycount_1 = 0;
  mycount_2 = 1;
  
  return 0;
}
