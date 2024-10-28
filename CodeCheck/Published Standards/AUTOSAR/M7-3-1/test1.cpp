// MISRA C++ 2023

#include <cstdint>

void f1( int32_t );     // UndCC_Violation

int32_t x1;             // UndCC_Violation

namespace               // UndCC_Valid
{
  void f2( int32_t );   // Rule does not apply

  int32_t x2;           // Rule does not apply
}

namespace MY_API        // UndCC_Valid
{
  void b2( int32_t );   // Rule does not apply

  int32_t x2;           // Rule does not apply
}

using namespace MY_API; // UndCC_Violation(a)
using MY_API::b2;       // UndCC_Violation(a)
namespace MY = MY_API;  // UndCC_Valid

int main()              // UndCC_Valid
{
  extern void f3();     // UndCC_Violation
}
