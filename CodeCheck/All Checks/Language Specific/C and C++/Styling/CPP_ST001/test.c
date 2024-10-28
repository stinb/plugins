#include <stdio.h>
int main()
{
  int a = 1; // UndCC_Valid
  int b = 2 ; // UndCC_Valid
  int c = 3  ; // UndCC_Violation

  if (a == b)
    a = 5;

  int condition_1 = 1;
  int condition_2 = 0;
  int condition_3 = 0     ; // UndCC_Violation
  int condition_4 = 1;
  if (condition_1 == 0 ||
      condition_2 == 0 ||
      condition_3 == 1)
    ;

  if(!condition_4)
    /* Comment explaining this null statement's purpose... */
    ;

  if (condition_1) {
    ;
  }

  switch (condition_1) {
    case 1:
      ;

    case 0:
      ;

    default:
      break;
  }




  return 0;
}

