// Custom

#include <stdint.h>

void f()
{
  uint32_t u32a, u32b;
  uint16_t u16a, u16b;

  u32a == u16a + u16b; // UndCC_Valid
  u32a *= u16a + u16b; // UndCC_Violation
}
