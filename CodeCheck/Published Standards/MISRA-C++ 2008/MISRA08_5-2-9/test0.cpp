// MISRA C++ 2008

#include <cstdint>
#define int32_t intptr_t
struct S
{
   int32_t i;
   int32_t j;
};

void f ( S * s )
{

   int32_t p = reinterpret_cast< int32_t >( s );   // UndCC_Violation

}
