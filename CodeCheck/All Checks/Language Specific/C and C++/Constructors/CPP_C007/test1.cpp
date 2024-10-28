#include <cstdint>

// Custom

class B                   // UndCC_Valid - Example 1 is explicitly declared
{
public:
  B ( );
  B ( B const & rhs );    // Example 1 - copy constructor
  template <typename T>
  B ( T const & rhs )     // Example 2
  : i ( new int32_t )
  {
    *i = *rhs.i;
  }
private:
  int32_t * i;            // Member requires deep copy
};
void f ( B const & b1 )
{
  B b2 ( b1 );            // Expectedly uses Example 1
}
