#include <stdio.h>

#define AGE 6   /* This inline comment MAY flag a violation */ // UndCC_Violation

int main () {

   /* This is a valid comment preceding int a */

   int a;   

   // This is also a valid comment preceding int b

   int b;   
   /* This is an INVALID comment following int b */ // UndCC_Violation

   int c;
   // And this is another INVALID comment following int c // UndCC_Violation
   
   if (c == 3) {
      c++;
   }
   /* This comment is valid, only character on preceding line is a closing bracket */


   return 0;
}
