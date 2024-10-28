#include <stdio.h>

#define MAX 100

int add(int num1, int num2);
void aDD(); // UndCC_Violation

int main() {

  int a;
  int b;

  int A = 3; // UndCC_Violation

  a = 4;
  b = 5;

  add(a, b);

  return 0;
}

int add (int num1, int num2) {

  return num1 + num2;

}

void aDD () {
  // does nothing
}
