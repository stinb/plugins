#include <stdarg.h>
#include <stdint.h>

void h ( va_list ap )             /* UndCC_Violation */
{
  double y;

  y = va_arg ( ap, double );      /* UndCC_Violation */
}

void f ( uint16_t n, ... )
{
  uint32_t x;

  va_list ap;                     /* UndCC_Violation */

  va_start ( ap, n );             /* UndCC_Violation */
  x = va_arg ( ap, uint32_t );    /* UndCC_Violation */

  h ( ap );

  /* undefined - ap is indeterminate because va_arg used in h ( ) */
  x = va_arg ( ap, uint32_t );    /* UndCC_Violation */

  /* undefined - returns without using va_end ( ) */
}

void g ( void )
{
  /* undefined - uint32_t:double type mismatch when f uses va_arg ( ) */
  f ( 1, 2.0, 3.0 );
}
