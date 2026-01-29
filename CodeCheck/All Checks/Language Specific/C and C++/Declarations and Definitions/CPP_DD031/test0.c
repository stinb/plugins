// MISRA C 2012 & 2023

#include <stdint.h>

uint16_t total ( uint16_t n, uint16_t a[ static 20 ] ) // UndCC_Violation
{
  uint16_t i;

  uint16_t sum = 0U;

  /* Undefined behaviour if a has fewer than 20 elements */
  for ( i = 0U; i < n; ++i )
  {
    sum = sum + a[ i ];
  }

  return sum;
}

extern uint16_t v1[ 10 ];
extern uint16_t v2[ 20 ];

void g ( void )
{
  uint16_t x;

  x = total ( 10U, v1 ); /* Undefined - v1 has 10 elements but needs
                          *              at least 20                  */
  x = total ( 20U, v2 ); /* Defined but non-compliant                 */
}
