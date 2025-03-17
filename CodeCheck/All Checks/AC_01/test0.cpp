// MISRA C++ 2008 and MISRA C++ 2023

#include <cstdint>

int32_t fn_3 ( int32_t x );

int32_t fn ( int32_t x ) // UndCC_Violation
{
  if ( x > 0 )
  {
    x = x * fn ( x - 1 );
  }
  return ( x );
}

int32_t fn_2 ( int32_t x ) // UndCC_Violation
{
  if ( x > 0 )
  {
    x = x * fn_3 ( x - 1 );
  }
  return ( x );
}

int32_t fn_3 ( int32_t x ) // UndCC_Violation
{
  if ( x == 0 )
  {
    x = x * fn_2 ( x - 1 );
  }
  return ( x );
}
