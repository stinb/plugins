// Custom

#include <cstdint>

void f1 ( )
{
  int32_t a1[ 10 ];
  int32_t a2[ 10 ];

  int32_t * p1 = &a1 [ 1 ];
  int32_t * p2 = &a2 [ 10 ];
  int32_t * p3 = &a1 [ 2 ];
  int32_t diff;

  diff = p2 - p1; // UndCC_Violation
  diff = p3 - p1; // UndCC_Valid
}

void f2(int *array)
{
  int *a = array;
  int *b = a;
  int *c = a + 1;
  b - c; // UndCC_Valid
}
