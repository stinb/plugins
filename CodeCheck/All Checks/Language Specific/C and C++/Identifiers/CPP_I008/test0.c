// MISRA C 2012 & 2023 (modified comments to flag each case)

#include <stdint.h>

static int32_t count;    // UndCC_Violation
static void foo ( void ) // UndCC_Violation
{
  int16_t count;         // UndCC_Violation
  int16_t index;         // UndCC_Valid
}

void bar1 ( void )       // UndCC_Valid
{
  static int16_t count;  // UndCC_Violation
  int16_t index;         // UndCC_Valid
  foo ( );
}
