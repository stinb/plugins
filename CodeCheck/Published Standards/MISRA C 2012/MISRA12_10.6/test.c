#include <stdint.h>

void use_uint32(uint32_t value, uint32_t value1) {
  // Function implementation
}

int main() {
  uint16_t u16a = 0;
  uint16_t u16b = 0;
  uint16_t u16c = 0;
  int a = 0;
  int b = 0;
  char c = 0;
  char d = 0;
  uint32_t u32a;
  int e = (int) c + d;
  u32a = u16a + u16b; /* UndCC_Violation Implicit conversion on assignment */
  use_uint32((uint32_t)u16a + u16b, (uint32_t)u16a + u16a); /* UndCC_Valid Implicit conversion of fn argument */
  use_uint32((uint32_t)u16a + u16b, u16a + u16a); /* UndCC_Violation Implicit conversion of fn argument */
  u16c = u16a + u16b; /* UndCC_Valid Same essential type */
  u32a = ( uint32_t ) u16a + u16b; // UndCC_Valid

  return 0;
}

