// Custom

#include <stdint.h>

void f()
{
  UINT8_C(100);  // UndCC_Violation
  UINT16_C(100); // UndCC_Violation(32)
  UINT32_C(100);
  UINT64_C(100);

  INT8_C(100);   // UndCC_Violation
  INT16_C(100);  // UndCC_Violation(32)
  INT32_C(100);
  INT64_C(100);
}
