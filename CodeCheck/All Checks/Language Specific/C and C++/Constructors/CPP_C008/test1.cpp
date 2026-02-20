// Custom

#include <stdint.h>

int global = 42;

static void effect ( )
{
   global++;
}

class B
{
public:
   B ( )
   {
   }

   B ( const B & rhs )
   : m_i ( rhs.m_i )
   {
      ++m_static;   // UndCC_Violation
      global = 0;   // UndCC_Violation
      int x = 0;    // UndCC_Valid - does not modify program state
      effect();     // UndCC_Violation
   }

   bool load( const B & rhs )
   {
      ++m_static;   // UndCC_Valid - not in a copy constructor
   }

private:
          int32_t m_i;
   static int32_t m_static;
};

int32_t B::m_static = 0;

B f ( )
{
   return B ( );
}

void b ( )
{
   B a = f ( );
}


class C
{
public:
   C( const int & number )
   {
      m_static++;   // UndCC_Valid - not in a copy constructor
      global = 0;   // UndCC_Valid - not in a copy constructor
      int x = 0;    // UndCC_Valid - does not modify program state
      effect();     // UndCC_Valid - not in a copy constructor
   }

private:
   static int32_t m_static;
};
