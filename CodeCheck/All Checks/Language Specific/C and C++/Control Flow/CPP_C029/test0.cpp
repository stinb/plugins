// MISRA C++ 2008

#include <cstdint>

#define expression 1

void fn ( void )      // UndCC_Violation(decl) - 1 early return
{
  if ( expression )
  {
    return;           // UndCC_Violation(exit) - early return
  }
  try
  {
    if ( expression )
    {
      throw ( 1 );
    }
  }
  catch ( int32_t )
  {
    throw;
  }
  return;
}
void fn2 ( void )     // UndCC_Violation(decl) - 2 early returns
{
  try
  {
    return;           // UndCC_Violation(exit) - early return
  }
  catch ( ... )
  {
    return;           // UndCC_Violation(exit) - early return
  }
}
void fn3 ( void ) try // UndCC_Valid - 1 return at each end
{
  return;
}
catch ( int32_t )
{
  return;
}
catch ( ... )
{
  return;
}
