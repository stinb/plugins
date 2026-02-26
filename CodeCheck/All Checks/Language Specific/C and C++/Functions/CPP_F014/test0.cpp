// MISRA C++ 2008

#include <stdint.h>

class A
{
public:
   int16_t f1 ( )   // UndCC_Violation - can be const
   {
      return m_i;
   }

   int16_t f2 ( )   // UndCC_Violation - can be static
   {
      return m_s;
   }
   int16_t f3 ( )   // UndCC_Valid - cannot be const or static
   {
      return ++m_i;
   }
private:
   int16_t m_i;
   static int16_t m_s;
};
