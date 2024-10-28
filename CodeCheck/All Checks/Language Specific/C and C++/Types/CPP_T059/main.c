#include <stdio.h>
#include "test.h"

typedef unsigned char uint_8; /* 8-bit unsigned integer */ // UndCC_Violation

uint_8 bad_var1; /* width-sensitive, should be placed in header file */

unsigned char bad_var2; /* not typedef'd or placed in header file */ // UndCC_Violation

int main() {
  
  unsigned char bad_var3; /* not typedef'd or placed in header file */ // UndCC_Violation
  
  return 0;
}
