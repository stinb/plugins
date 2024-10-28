#include <cstdint>

class D
{
public:
  int32_t x;
  static int32_t y;
  D ( )
    try
    {
      // Action that may raise an exception
    }
    catch ( ... )
    {
      if ( 0 == x ) // UndCC_Violation - x may not exist at this point
      {
      // Action dependent on value of x
      }
    }
  ~D ( )
    try
    {
      // Action that may raise an exception
    }
    catch ( ... )
    {
      if ( 0 == y ) // UndCC_Valid - y is static
      {
        // Action dependent on value of y
      }
    }
  void other ( )
    try
    {
    // Action that may raise an exception
    }
    catch ( ... )
    {
      if ( 0 == x ) // UndCC_Valid - not in a constructor/destructor
      {
        // Action dependent on value of x
      }
    }
};
