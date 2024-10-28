#include <stdio.h>

int main() {
  unsigned int a = 60;
  unsigned int b = 13;	
  int c = 0;

  int badInt;        // not initialized, flag violation // UndCC_Violation
  float badFloat;    // not initialized, flag violation // UndCC_Violation
  badInt = 0;
  
  int goodInt = 0;   // UndCC_Valid
  float goodFloat = 0.0; // UndCC_Valid
  goodInt = 1;

  return 0;
}
