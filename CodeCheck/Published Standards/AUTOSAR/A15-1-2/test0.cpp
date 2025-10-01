// MISRA C++ 2008

#include <stdint.h>

class A
{
   // Implementation
};

void fn ( int16_t i )
{
   static A   a1;
          A * a2 = new A;

   if ( i > 10 )
   {
      throw ( &a1 );   // UndCC_Violation – pointer type thrown
   }
   else
   {
      throw ( a2 );    // UndCC_Violation – pointer type thrown
   }
}
