#include <stdio.h>
#include <stdlib.h>

int main() {
  char user_input[100];
  fgets(user_input, 100, stdin);
  system(user_input);  // UndCC_Violation
  return 0;
}

