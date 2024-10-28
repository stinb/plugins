// MISRA C++ 2008

#include <cstdint>

void f1 ( )
{
   int32_t * i = new int32_t;   // UndCC_Violation
   delete i;                    // UndCC_Violation
}
