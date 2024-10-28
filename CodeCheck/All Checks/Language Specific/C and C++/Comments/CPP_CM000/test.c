#include <stdio.h>
#define MAX_MINUTES 10

int main() {

 /* This is an invalid comment for the following line of code, it is not aligned to same column */
  int seconds = 180; // UndCC_Violation

  /* This is a comment for the following line of code */
  int minutes = seconds/60;


  if (minutes > MAX_MINUTES) {

    /* This is a comment for the following line of code */
    printf("Time exceeded maximum allowable time\n");

  }

  else {

    printf("Time did not exceed maximum allowable time\n");   /* UndCC_Violation This inline comment is invalid */

  }

  return 0;
}
