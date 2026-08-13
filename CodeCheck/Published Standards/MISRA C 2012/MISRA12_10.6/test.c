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

// An 8-bit destination is not wider than a 16/32-bit operand. Earlier the
// size comparison was done on strings ("32" < "8"), so any wider operand was
// treated as narrower and these were flagged.
void narrow_destination(void) {
  uint8_t  u8;
  int32_t  state = 2;
  uint8_t  dataArr[4] = {0x73, 0xA8, 0xFF, 0x13};
  uint8_t *dataPtr = &dataArr[0];

  u8 = dataPtr[state] & 0x80;  // UndCC_Valid

  uint8_t *pOut;
  int32_t  n = 3;
  pOut = &dataPtr[(uint32_t)n >> 3U];  // UndCC_Valid
}


/* The operands of a composite expression are its own, not those of a call it
   contains, and only a composite expression is covered by this rule */
#define FLAG_A 1
#define FLAG_B 2

uint16_t crc16(const uint8_t *data, int32_t length);
uint32_t slice(const uint8_t *data, uint32_t offset, uint32_t len);

void compositeOperands(void) {
  uint8_t  dataArr[4] = {0};
  uint16_t crc;
  uint32_t result;
  uint32_t flags;
  uint8_t *p;

  crc = crc16(dataArr, 8) & 0x0FFU;   // UndCC_Valid - the uint8_t argument is not an operand
  result = slice(dataArr, 7, 16) << 16; // UndCC_Valid - a shift keeps the left operand's type
  result = slice(dataArr, 7, 16);     // UndCC_Valid - a call is not a composite expression
  flags = FLAG_A | FLAG_B;            // UndCC_Valid - a constant expression is not composite
  p = &dataArr[0];                    // UndCC_Valid - a pointer is outside the essential type model
}
