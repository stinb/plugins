// MISRA C++ 2023

#include <stdint.h>

void f( bool b )
{
  uint16_t x = 0U;   // UndCC_Violation \
  if ( b )
  {
    ++x;             // This is always executed
  }
}
