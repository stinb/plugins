// MISRA C 2012 & 2023 (modified comments to flag each case)

#include <stdint.h>

static int8_t count;     // UndCC_Violation
static void foo ( void ) // UndCC_Violation
{
  int32_t index;         // UndCC_Valid
  int16_t nbytes;        // UndCC_Valid
}

void bar2 ( void )       // UndCC_Valid
{
  static uint8_t nbytes; // UndCC_Valid
}
