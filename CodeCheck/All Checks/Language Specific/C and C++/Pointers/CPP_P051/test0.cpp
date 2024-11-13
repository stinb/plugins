// MISRA C++ 2008

#include <cstdint>

struct S
{
   int32_t i;
   int32_t j;
};

void f ( S * s )
{
#ifdef _WIN32
   int32_t p = reinterpret_cast< int32_t >( s );   // UndCC_Violation(Win)
#endif
}

