#include <stdio.h>
#include <stdlib.h>

int main() {
  
  int age = 10, height = 45;

  /* valid */
  if ((age < 12 && height < 48) || (age > 65 && height > 72)) {
    // do nothing
  }

  /* invalid, insufficient use of parentheses in complex logical expression */
  if (age < 12 && height < 48 || age > 65 && height > 72) { // UndCC_Violation
    // do nothing
  }
  
  /* invalid, arithmetic expression relies on operator precedence */
  age = height * 2 / 4; // UndCC_Violation
  
  
  /* random expression for testing */
  if (age < 12 && height < 48 * 2 + 1 || (age > 65 && height > 72)) { // UndCC_Violation
    // do nothing
  }
  
  return 0;
}
