#include <stdio.h>

// UndCC_Valid
int * validTest() {
  int * a = NULL;
  int b = 1;
  int * c = &b;
  if (b) {
    return c;
  }
  return a;
}


int * invalidTest() { // UndCC_Violation
  int a = 1;
  int * b = &a;
  int c = 2;
  int * d = &c;
  
  if (a) {
    return b;
  }
  return d; // does not return null pointer if case was not successful
}
