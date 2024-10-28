// MISRA C 2012

#include <stddef.h>
#include <stdint.h>

volatile int32_t i;
         int32_t j;
         size_t s;

void e ( void )
{
  s = sizeof ( j );       /* UndCC_Valid */
  s = sizeof ( j++ );     /* UndCC_Violation */
  s = sizeof ( i );       /* UndCC_Valid - exception */
  s = sizeof ( int32_t ); /* UndCC_Valid */
}

volatile uint32_t v;

void f ( int32_t n )
{
  size_t s;

  s = sizeof ( int32_t[ n ] );                       /* UndCC_Valid */
  s = sizeof ( int32_t[ n++ ] );                     /* UndCC_Violation */
  s = sizeof ( void ( *[ n ] ) ( int32_t a[ v ] ) ); /* UndCC_FalseNeg */
}
