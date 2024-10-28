#include <stdio.h>

/* Global variables */
char *file_name;

int   temp0;
      temp1; /* No type specified - defaults to int - INVALID */ // UndCC_Violation
	 *temp2; /* Also invalid */  // UndCC_Violation
float tempFloat;

int main() {

   int myInt;
   
   float* anotherFloat;
   
   return 0;
}
