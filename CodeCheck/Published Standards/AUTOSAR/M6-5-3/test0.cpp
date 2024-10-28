// MISRA C++ 2008

#include <stdint.h>

bool modify ( int32_t * pX )
{
  *pX++;
  return ( *pX < 10 );
}

void fn () {
  int32_t x;

  for ( x = 0; modify ( &x ); ) // UndCC_Violation
  {
  }
  for ( x = 0; x < 10; )
  {
    x = x * 2; // UndCC_Violation
  }
}
