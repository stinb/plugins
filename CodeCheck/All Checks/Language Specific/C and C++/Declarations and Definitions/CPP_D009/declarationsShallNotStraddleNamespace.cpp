namespace NS1_735
{
  void f_735( unsigned short int ); // Example 1
}

using NS1_735::f_735; // UndCC_Violation

namespace NS1_735
{
  void f_735( unsigned int ); // Example 2 � Non-compliant
}

void bar_735()
{
  f_735( 0U );
}
