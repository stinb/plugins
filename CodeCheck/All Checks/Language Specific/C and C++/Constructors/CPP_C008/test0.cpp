// MISRA C++ 2008

#include <stdint.h>

class A
{
public:
   A ( )
   {
   }

   A ( A const & rhs )
   : m_i ( rhs.m_i )
   {
      ++m_static;   // UndCC_Violation
   }

private:
          int32_t m_i;
   static int32_t m_static;
};

int32_t A::m_static = 0;

A f ( )
{
   return A ( );
}

void b ( )
{
   A a = f ( );
}
