#include <stdio.h>
#include <stdlib.h>

int main() {
  
  int i = 0;  /* counter variable */
  int total = 0;  /* total number of items */
  int test = 0;
  int test2 = 0;

  for (i = 0; i < 10; i++) {

    total = total * ++total;  /* invalid, total is altered more than once */ // UndCC_Violation
    
    test = test + test;
    test++;

  }
  
  /* example 2, passing by reference */
  
  int a = 100;
  
  a = a + func(&a);          /* invalid, a passed by ref and could be altered */ // UndCC_Violation
  a = 3;
  
  return 0;
}

int func(int *x) {

  *x = *x + 3;
  
  return x;
}
