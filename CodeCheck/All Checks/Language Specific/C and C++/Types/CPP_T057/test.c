#include <stdio.h>

void func_x (double arg) {

  // ...

}

void example (void) {
  int y = 5;
  func_x ((double)y);  // UndCC_Valid - type cast argument y in the call to x

  return;
}

void example_2 (int a, double b) {

  // ...

}

int main () {

  int hi = 1;
  int hello = 2;

  example_2(hi, hello);  // UndCC_Violation - 'hello' not explicitly cast to double

  return 0;
}


