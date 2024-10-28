#include <stdio.h>

int sum(a, b) {
  return a + b;
}

int main() {
  
  int i = 0;
  int count = 0;
  int total;
  
  for (i; i < 10; i++) {
    count++;  
  }
  
  for (i; i < 5; i++)
  {
  count--; // invalid indentation // UndCC_Violation
char e = "E"; // another invalid indentation // UndCC_Violation
  }
  
  if (count == 6) {
  //comments should not be flagged, so this line is valid
    if (i == 0) {
    total = sum(count, 4); // invalid indentation on a nested if statement // UndCC_Violation
    }
  int v; // invalid indentation after a nested control structure // UndCC_Violation
  }
  
  if (i == 0) {
    // do this
  } else {
    i++;   // do not flag this, it is OK for statement to align to 'else' in this case
  }
  
  return 0;
}
