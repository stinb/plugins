// usingDirectiveDeclarationsNotInHeaderFiles1.h

namespace NS2_736
{
}

void foo ( char a );

namespace NS1
{
  void foo( int a );
}
inline void bar ( )
{
  using namespace NS2_736; // UndCC_Violation
  foo ( 0 );
}
