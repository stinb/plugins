#include <stdio.h>

int main () {
    
  if (0) {
    //valid
  }
  
  int a = 0;
  if (a) { // UndCC_Violation
    //invalid
  } // UndCC_Violation
  a = 1;
  
  return 0;
}

