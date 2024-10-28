#include <stdio.h>

int main()
{
  int a = 1;
  int b = 2;
  int c = 3;
  a += c; // UndCC_Violation
  a        = 2; // UndCC_Violation


  if (a == b) {
    a = NULL;
  }
  
  return 0;
}




