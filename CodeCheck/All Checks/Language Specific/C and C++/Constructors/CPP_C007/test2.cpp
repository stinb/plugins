#include <cstdint>

// Custom

class C                   // UndCC_Valid - Example 2 is not declared
{
public:
  C ( );
  // C ( C const & rhs );    Example 1 - implicitly generated
  // template <typename T>
  // C ( T const & rhs )     Example 2
  // : i ( new int32_t )
  // {
  //   *i = *rhs.i;
  // }
private:
  int32_t * i;            // Member requires deep copy
};
void f ( C const & c1 )
{
  C c2 ( c1 );            // Expectedly uses Example 1
}
