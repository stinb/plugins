// MISRA C 2012

#include <stdint.h>

void use_int16 ( int16_t );

void unused_label ( void )
{
  int16_t x = 6;
label1: // UndCC_Violation
  use_int16 ( x );
}
