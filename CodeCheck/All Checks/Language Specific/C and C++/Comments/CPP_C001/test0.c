// MISRA C 2012 & 2023

#include <stdbool.h>
#include <stdint.h>

typedef bool bool_t;

extern bool_t b;

void f ( void )
{
  uint16_t x = 0;   // UndCC_Violation \
  if ( b )
  {
    ++x;            /* This is always executed */
  }
}
