// MISRA 2008 6-6-4

#include <cstdint>

void test0 ()
{
  bool condition = true;

  for ( int32_t i = 0; i < 10; i++ ) // UndCC_Valid - 1 break
  {
    if ( condition )
    {
      break;
    }
  }
  while ( condition )                // UndCC_Valid - 1 break
  {
    if ( condition )
    {
      break;
    }
  }
  for ( int32_t i = 0; i < 10; i++ ) // UndCC_Violation(break, both) - 2 breaks
  {
    if ( condition )
    {
      break;
    }
    else if ( condition )
    {
      break;
    }
    else
    {
      // ...
    }
  }
  while ( condition )                // UndCC_Violation(break, both) - 2 breaks
  {
    if ( condition )
    {
      break;
    }
    if ( condition )
    {
      break;
    }
  }
}
