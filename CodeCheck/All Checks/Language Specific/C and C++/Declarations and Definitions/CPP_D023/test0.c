// MISRA C 2012/2023


#include <stdint.h>

#define N 10 // Used to make func not have an error


void func ( void )
{
  int32_t i; // UndCC_Valid

  for ( i = 0; i < N; ++i )
  {
  }
}


uint32_t count ( void )
{
  static uint32_t call_count = 0; // UndCC_Valid

  ++call_count;
  return call_count;
}
