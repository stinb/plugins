// Custom

#include <stdint.h>
#include <string.h>

void f(
  const uint8_t a8[8], uint8_t b8[9], uint8_t *c8,
  uint16_t a16[8], uint16_t b16[9], uint16_t *c16)
{
  memcmp(a8, a8, 8);  // UndCC_Valid
  memcmp(a8, b8, 8);  // UndCC_Valid
  memcmp(a8, c8, 8);  // UndCC_Valid
  memcmp(a8, a16, 8); // UndCC_Violation
  memcmp(a8, b16, 8); // UndCC_Violation
  memcmp(a8, c16, 8); // UndCC_Violation

  memcpy(a8, a8, 8);  // UndCC_Valid
  memcpy(a8, b8, 8);  // UndCC_Valid
  memcpy(a8, c8, 8);  // UndCC_Valid
  memcpy(a8, a16, 8); // UndCC_Violation
  memcpy(a8, b16, 8); // UndCC_Violation
  memcpy(a8, c16, 8); // UndCC_Violation

  memmove(a8, a8, 8);  // UndCC_Valid
  memmove(a8, b8, 8);  // UndCC_Valid
  memmove(a8, c8, 8);  // UndCC_Valid
  memmove(a8, a16, 8); // UndCC_Violation
  memmove(a8, b16, 8); // UndCC_Violation
  memmove(a8, c16, 8); // UndCC_Violation
}
