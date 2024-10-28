#include <cstdint>

class E
{
public:
  int32_t x;
  static int32_t y;
  E ( )
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
  ~E ( )
    try
    {
      // Action that may raise an exception
    }
    catch ( ... )
    {
      if ( 0 == getX() ) // UndCC_Violation - x may not exist at this point
      {
        // Action dependent on value of x
      }
	  if ( 0 == y ) // UndCC_Valid - y is static
      {
		  // Action dependent on value of y
      }
    }
  E ( int a )
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
	int32_t getX() {
		return x;
	}
};
