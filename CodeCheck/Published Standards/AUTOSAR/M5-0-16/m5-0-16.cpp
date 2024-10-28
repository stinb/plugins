#include <stdlib.h>
#include <cstdint>
void f1 ( const int32_t * a1 )
{
  int a = 10;
  int32_t a2[ 10 ];
  const int32_t * p = &a1 [ 1 ]; // UndCC_Violation - a1 not an array
  int32_t * p2 = &a2 [ 10 ];     // UndCC_Valid
  int32_t * p3 = &a2 [ 9 ];      // UndCC_Violation

}
void f2 ( )
{
  int32_t b;
  int32_t c [ 10 ];
  f1 ( &b );
  f1 ( c );
}
