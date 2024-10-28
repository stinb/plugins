#include <stdio.h>
//#include "test.c"
// When test.c is not #included, this check should flag violations for the entities used across both files

struct Car car2;
enum week day2;

int main() {
  
  /* Use all of the objects */
  if (addNumbers(3,5) == 8) { // UndCC_Violation
    printf("Correct!");
  }
  
  BYTE b2 = 2;
  
  printf("%d", b2);
  

  car2.miles = KILOMETER;
  if (car2.miles == 1000) {
    printf("%d\n", car2.miles);
  }
  
  printf("%d\n",day2);
  
  return 0;
}
