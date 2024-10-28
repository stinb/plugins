#include <stdio.h>

int main() {
  int counter = 0;
  int i = 0;
  int k;
    
  /* good for loop */
  for (k = 0; k <= 6; k++) {
    
    // some code...
    
  }   
    
    
  /* good while loop */
  while (counter <= 6) {

    // some code...
    
    counter++;    
  }
  
    
  /* good do loop */
  do {
      i++;
      
      // some code...

    } while (i <= 6);
    
    
  /* bad for loop */
  for (k = 0; k <= 6; k++) {
    k++; // UndCC_Violation
  }
    
    
  /* bad while loop */
  while (counter <= 6) {

    // some code...
    
    counter++; // UndCC_Violation
    
    // some code...
    
  }
   
    
  /* bad do loop */
  do {
    
    // some code...
    
    i++; // UndCC_Violation
    
    // some code...

  } while (i <= 6);
  
    
  return 0;
}
