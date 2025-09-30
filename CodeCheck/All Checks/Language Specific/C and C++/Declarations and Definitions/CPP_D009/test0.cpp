// MISRA C++ 2008

namespace NS1
{
  void f( unsigned short int ); // Example 1
}

using NS1::f; // UndCC_Violation

namespace NS1
{
  void f( unsigned int ); // Example 2 - Non-compliant
}

void bar()
{
  f( 0U );
}
