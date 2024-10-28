// MISRA C++ 2008

#include <cstdint>

template < typename T>
class A
{
  public:
    template < int32_t i >
    void f2 ( );
};

void f ( A<int32_t> * a<:10:> )  // UndCC_Violation
<% a<:0:>->f2<20> ( ); %>        // UndCC_Violation

// The above is equivalent to:
void f1 ( A<int32_t> * a[ 10 ] )
{
  a[ 0 ]->f2<20> ( );            // UndCC_Valid
}
