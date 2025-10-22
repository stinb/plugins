// MISRA C 2023

#include <stdbool.h>

void f( int x, int y, int z )
{
  switch ( x == 0 )  /* UndCC_Violation - essentially Boolean */
  {                  /* In this case an "if-else" would be more logical */
    case false:
      y = x;
      break;
    default:
      y = z;
      break;
  }
}
