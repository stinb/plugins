// MISRA C 2023

#include <stdint.h>

void f1 (uint16_t n, uint16_t (* a) [n]) // UndCC_Violation
{
  uint16_t ( *p )[ 20 ];
  p = a; /* undefined unless n == 20, but types always assumed compatible */
}

void f2 (uint16_t n, uint16_t a[n])      // UndCC_Valid
{
  uint16_t * p;
  p = a; /* pointed-to type is not variably-modified, always well-defined */
}
