#include <stdio.h>


int main() {
  int *myPointer; // VALID
  int ***myTriplePointer; // VALID
  int *myNum = 10; // VALID
  char* myChar;  // INVALID // UndCC_Violation
  char * myOtherChar; // INVALID // UndCC_Violation

  return 0;
}
