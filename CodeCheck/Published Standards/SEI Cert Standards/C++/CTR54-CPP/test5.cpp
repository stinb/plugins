// MISRA C++ 2008 - 5-0-17

#include <cstdint>

void f1 ( )
{
  int32_t a1[ 10 ];
  int32_t a2[ 10 ];

  int32_t * p1 = &a1 [ 1 ];
  int32_t * p2 = &a2 [ 10 ];
  int32_t diff;

  diff = p1 - a1; // UndCC_Valid
  diff = p2 - a2; // UndCC_Valid
  diff = p1 - p2; // UndCC_Violation
}
