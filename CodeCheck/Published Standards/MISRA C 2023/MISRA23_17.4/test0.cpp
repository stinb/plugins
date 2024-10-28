// MISRA C++ 2008

#include <cstdint>

int32_t fn1 ( void )
{
} // UndCC_Violation
int32_t fn3 ( int32_t x )
{
   if ( x > 100 )
   {
      throw 42;
   }
   return ( x );
} // UndCC_Valid by exception
