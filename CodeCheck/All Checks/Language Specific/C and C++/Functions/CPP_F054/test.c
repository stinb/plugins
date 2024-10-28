#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

bool is_mult_of_five_implicit(int n) {     // UndCC_Violation
  return n % 5 == 0;
}

bool is_mult_of_five_explicit(int n) {
  if (n % 5 == 0)
    return true;
  else
    return false;
}

bool is_not_mult_of_five_implicit(int n) { // UndCC_Violation
  return !is_mult_of_five_implicit(n);
}

bool is_not_mult_of_five_explicit(int n) {
  if (n % 5 != 0)
    return true;
  else
    return false;
}

int main() {
  printf("Enter the number you want to check: ");

  char buffer[256];
  fgets(buffer, sizeof buffer, stdin);
  int num = atoi(buffer);

  if (is_mult_of_five_implicit(num))
    printf("Number is a multiple of 5\n");
  else
    printf("Number isn't a multiple of 5\n");

  return 0;
}
