#include <stdio.h>


int main() {
  int *myPointer;         // UndCC_Valid
  int ***myTriplePointer; // UndCC_Valid
  int *myNum = 10;        // UndCC_Valid
  char* myChar;           // UndCC_Violation
  char * myOtherChar;     // UndCC_Violation

  return 0;
}
