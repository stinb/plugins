#include <stdio.h>

int main()
{
  int a = 1;

  switch(a) {
    case 1 :
      printf("Case 1");
      break;
    case 2 :
    case 3 :
     printf("Case 3"); // UndCC_Violation
     break;
    case 4 :
       printf("Case 4"); // UndCC_Violation
       break;
    case 5 :
      printf("Case 5");
      break;
    default :
      printf("Default");
  }
  printf("");
  
  return 0;

}


