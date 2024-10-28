// MISRA C++ 2008

#include <cstdint>

struct S
{
   intptr_t i;
   intptr_t j;
};

void f ( S * s )
{
#if defined(_WIN32) || defined(__APPLE__)
   intptr_t p = reinterpret_cast< intptr_t >( s );   // UndCC_Violation(Win,Mac)
#endif
}
