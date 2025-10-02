// MISRA C++ 2008

#include <stdint.h>

class C
{
public:
   int32_t & getA ()   // UndCC_Violation
   {
      return a;
   }
private:
   int32_t a;
};

void b ( C & c )
{
   int32_t & a_ref = c.getA ();
   a_ref = 10;                  // External modification of private C::a
}
