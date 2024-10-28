#include <stdio.h>

int addNums (int a, int b) {  // UndCC_Valid
   int result;
   result = a + b;
   return result;
}
// Add two numbers together and return the sum



// There is no comment identifying the end of subNums, so it will be flagged by Codecheck
int subNums (int c, int d) {  // UndCC_Violation
   int result;
   result = c - d; 
   return result;
}

