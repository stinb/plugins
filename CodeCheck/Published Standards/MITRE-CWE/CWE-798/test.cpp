#include <stdio.h>
#include <string.h>

int VerifyAdmin(char *password) {
  if (strcmp(password, "Mew!") == 0) { // UndCC_Violation
    printf("Entering Diagnostic Mode...\n");
    return 1;
  }
  printf("Incorrect Password!\n");
  return 0;
}
