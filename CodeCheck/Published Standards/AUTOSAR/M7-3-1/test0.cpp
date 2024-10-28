// MISRA C++ 2008

#include <cstdint>

void f1 ( int32_t );    // UndCC_Violation

int32_t x1;             // UndCC_Violation

namespace
{
   void f2 ( int32_t ); // UndCC_Valid
   int32_t x2;          // UndCC_Valid
}

namespace MY_API
{
   void b2 ( int32_t ); // UndCC_Valid
   int32_t x2;          // UndCC_Valid
}

int32_t main ( )        // UndCC_Valid
{
}
