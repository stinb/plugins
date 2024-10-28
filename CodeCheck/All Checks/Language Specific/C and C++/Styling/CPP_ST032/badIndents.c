#include <stdio.h>

int main() {
// Valid (with standard set to 3 spaces per indentation)
   if(0) {
      while (0) {
         // do something
      }
   }
   else
      return 0;

// Invalid (with standard set to 3 spaces per indentation)
  if(0) {             //UndCC_Violation 2 space indentation - line flags violation
      while (0) {    
         // do something 
      }               
   }
   else
    return 0;           //UndCC_Violation 4 space indentation - line flags violation

   return 0;
}
