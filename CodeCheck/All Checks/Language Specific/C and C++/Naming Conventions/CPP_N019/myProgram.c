#include <stdio.h>

int okGlobal = 1;
static int ABADGLOBAL; // UndCC_Violation - will only flag violation if option to ignore const/static names is not checked


// this function will flag violation if option to ignore const/static names is not checked
const int MIN(int num1, int num2) { //UndCC_Violation

  /* local variable declaration */
  int result;

  if (num1 < num2)
    result = num1;
  else
    result = num2;

  return result;
}

int main() {
   float BAD_LOCAL = 4.3; //UndCC_Violation
   float _ANOTHER_BAD_ONE = 4.4; //UndCC_Violation

   float goodLocal = 2.5; //UndCC_Valid

   float another_good_local = 2.3; //UndCC_Valid

   return 0;
}
