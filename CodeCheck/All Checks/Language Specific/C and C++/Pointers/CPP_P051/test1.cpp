// MISRA C++ 2023

#include <cstdint>

struct S;

void f( S * s )
{
  std::intptr_t p = reinterpret_cast< std::intptr_t >( s );   // UndCC_Violation
  std::uint8_t  q = reinterpret_cast< std::uint8_t  >( s );   // UndCC_Violation
}
