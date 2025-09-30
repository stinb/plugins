#include <stdio.h>

void main() {
  int n;
  do {
    printf(" \nEnter the number :");
    scanf("%d", &n);
    if (n < 0) {
      break;
    }
    if (n >10) {
      printf("Skip the value\n");
      continue; // UndCC_Violation
    }
    printf("The number is: %d", n);
  } while (n!= 0);
}
