// MISRA C++ 2023

#include <cstdint>

struct S;

void f( S * s )
{
  std::intptr_t p = reinterpret_cast< std::intptr_t >( s );   // UndCC_Violation
#if defined(_WIN32) || defined(__APPLE__)
  std::intptr_t  q = reinterpret_cast< std::intptr_t  >( s );   // UndCC_Violation(Win,Mac)
#endif
}
