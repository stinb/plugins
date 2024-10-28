#include <stdio.h>

// Indirect inclusion
#include "test3.h"

static void f() {
  int i;
  char szInput [256];
  printf("Enter a number: ");
  fgets(szInput, 256, stdin);
  i = atoi(szInput); // UndCC_Violation
  printf("The value entered is %d. The double is %d.\n", i, i * 2);
}
