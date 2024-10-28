/* Fill in the user’s buffer with the data read from the file. The
  data in the buffer includes the number of records and the maximum
  record size. */

/* Fill in the user’s buffer with the data read from the file. The // UndCC_Violation
data in the buffer includes the number of records and the maximum
record size. */

#include <stdio.h>

void func(int a,
          int b,
          int c);

void func1(int a,
int b); // UndCC_Violation

int main()
{
  if (0 &&
      1) {
    return 0;
  }
  if (0 &&
  1) { // UndCC_Violation
    return 0;
  }
  
  int a = (7 +
           3);
  
  int b = (3 +
  7); // UndCC_Violation
  
  char first_name[15],
       last_name[30],
       middle_name[15];
  
  char first_name1[15],
       last_name1[30],
  middle_name1[15]; // UndCC_Violation

  printf("Function Call",
         5);
  
  printf("Function Call",
  5); // UndCC_Violation
}
