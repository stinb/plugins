// MISRA C 2012 & 2023

#include <stdint.h>

void compliant()
{
  uint32_t u32a, u32b;
  uint16_t u16a, u16b;

  u32a * u16a + u16b;                  /* No composite conversion */
  ( u32a * u16a ) + u16b;              /* No composite conversion */
  u32a * ( ( uint32_t ) u16a + u16b ); /* Both operands of * have
                                        * same essential type */
  u32a += ( u32b + u16b );             /* No composite conversion */
}

void noncompliant()
{
  uint32_t u32a;
  uint16_t u16a, u16b;

  u32a * ( u16a + u16b );  /* UndCC_Violation - Implicit conversion of ( u16a + u16b ) */
  u32a += ( u16a + u16b ); /* UndCC_Violation - Implicit conversion of ( u16a + u16b ) */
}
