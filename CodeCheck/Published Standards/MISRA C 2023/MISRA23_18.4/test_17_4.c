// MISRA C 2004 Rule 17.4 example (strict mode)

#include <stdint.h>

void my_fn(uint8_t * p1, uint8_t p2[])
{
  uint8_t index = 0U;
  uint8_t * p3;
  uint8_t * p4;
  *p1 = 0U;
  p1 ++;              /* UndCC_Violation(strict) - pointer increment */
  p1 = p1 + 5;        /* UndCC_Violation - pointer arithmetic */
  p1[ 5 ] = 0U;       /* UndCC_Violation(strict) - p1 not declared as an array */
  p3 = &p1[ 5 ];      /* UndCC_Violation(strict) - p1 not declared as an array */
  p2[ 0 ] = 0U;       /* UndCC_Valid */
  index ++;           /* UndCC_Valid - index is not a pointer */
  index = index + 5U; /* UndCC_Valid - index is not a pointer */
  p2[ index ] = 0U;   /* UndCC_Valid */
  p4 = &p2[ 5 ];      /* UndCC_Valid */
  (void) p3;
  (void) p4;
}

void my_caller(void)
{
  uint8_t a1[ 16 ];
  uint8_t a2[ 16 ];
  uint8_t a[ 10 ];
  uint8_t * p;
  my_fn( a1, a2 );
  my_fn( &a1[ 4 ], &a2[ 4 ] );
  p = a;
  *( p + 5 ) = 0U;    /* UndCC_Violation - pointer arithmetic */
  p[ 5 ] = 0U;        /* UndCC_Violation(strict) - p not declared as an array */
}
