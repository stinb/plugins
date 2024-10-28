#include <stdio.h>


// should be placed in an include file
int foo(const int y) { // UndCC_Violation
  printf("Value is %d\n", y);
  
  return y; // UndCC_Violation
}


int main() {
  const int x = 10;
  
  int new_var = foo(x);

  return 0;
}
