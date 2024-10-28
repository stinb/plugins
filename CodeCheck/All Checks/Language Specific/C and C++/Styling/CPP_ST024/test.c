#include <stdio.h>

int add(int, int);
int sub(int, int);
int div(int, int);
int mult(int, int);

int main() {

  int a = 10;
  int b = 5;

  add(a, b);
  sub(a, b);
  div(a, b);
  mult(a, b);

  return 0;
}

int add(int num1, int num2) {    /* valid, blank line preceding function definition */
  return num1 + num2;
}
int sub(int num1, int num2) {    /* invalid */ // UndCC_Violation
  return num1 - num2;
}
/* invalid */
int div(int num1, int num2) { return num1 / num2; } // UndCC_Violation
int mult(int num1, int num2) {   /* invalid, non-blank line preceding mult() */ // UndCC_Violation
  return num1 * num2;
}
