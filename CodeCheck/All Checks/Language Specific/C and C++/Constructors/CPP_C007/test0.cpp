#include <cstdint>

// MISRA C++ 2008

class A                   // UndCC_Violation - Example 1 is not explicitly declared
{
public:
  A ( );
  // A ( A const & rhs );    Example 1 - implicitly generated
  template <typename T>
  A ( T const & rhs )     // Example 2
  : i ( new int32_t )
  {
    *i = *rhs.i;
  }
private:
  int32_t * i;            // Member requires deep copy
};
void f ( A const & a1 )
{
  A a2 ( a1 );            // Unexpectedly uses Example 1
}
