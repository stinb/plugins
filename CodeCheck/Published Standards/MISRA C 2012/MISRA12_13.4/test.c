#include <stdio.h>
#include <stdbool.h>

int f() {
  return 0; // dummy function for demonstration
}

int main() {
  int x, y, a[10], b, c;
  int bool_var;

  x = y; // UndCC_Valid

  a[x] = a[x = y]; // UndCC_Violation - the value of x = y is used

  if (bool_var = false) { // UndCC_Violation - value of bool_var = false is used but bool_var == false was probably intended
  }

  if ((0u == 0u) || (bool_var = true)) { // UndCC_Violation even though bool_var = true isn't evaluated
  }

  if ((x = f()) != 0) { // UndCC_Violation - value of x = f() is used
  }

  a[b += c] = a[b];   // UndCC_Violation - value of b += c is used
  int e;
  e = b = c = 0;  // UndCC_Violation - values of c = 0 and b = c = 0 are used


  return 0;
}

