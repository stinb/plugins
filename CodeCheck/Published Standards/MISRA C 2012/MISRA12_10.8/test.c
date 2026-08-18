#include <stdint.h>
#include <string.h>

// Rule 10.8 only applies to composite expressions. A cast of a single
// object, constant, function call or array element is compliant.
#define OUTPUT_BITS (256)
void non_composite_operands(const uint8_t *data, uint32_t index) {
  char aString[] = "abc";
  int32_t n;

  n = (int32_t)OUTPUT_BITS - 1;            // UndCC_Valid  cast of a constant
  n = (int32_t)strlen(aString);            // UndCC_Valid  cast of a function call
  uint64_t packed = (uint64_t)(data[index]); // UndCC_Valid  cast of an array element
  (void)n; (void)packed;
}

int main() {
  uint32_t u32a = 1, u32b = 2;
  short  s32a = 3, s32b = 4;
  uint16_t u16a = 5, u16b = 6;

  uint16_t result1 = (uint16_t) (u32a + u32b); // UndCC_Valid 
  uint32_t result2 = (uint32_t) (u16a + u16b); // UndCC_Violation
  uint16_t result3 = (uint16_t) (s32a + s32b); // UndCC_Violation
  uint16_t result4 = (uint16_t) s32a; // UndCC_Valid No composite expression


  ( uint16_t ) ( u32a + u32b ); /* UndCC_Valid */
  ( uint16_t ) ( s32a + s32b ); /* UndCC_Violation - different essential
* type category */
  ( uint16_t ) s32a; /* UndCC_Valid - s32a is not composite */
  ( uint32_t ) ( u16a + u16b ); /* UndCC_Violation - cast to wider
* essential type */

  // Pointer casts should not be checked
  #define DCI_TX_TRIG3_RAM_ADR (0x8000U + 0x48)
  const uint8_t * dci_tx_trig3_adr = (const uint8_t * )(DCI_TX_TRIG3_RAM_ADR); // UndCC_Valid
  const uint8_t * dci_tx_trig3_adr2 = (const uint8_t * )DCI_TX_TRIG3_RAM_ADR; // UndCC_Valid

  return 0;
}


// A cast of a composite expression to a narrower type of the same essential
// type category is permitted, a cast to a different category is not
void narrowerAndDifferentCategory(uint32_t u32) {
  uint8_t  u8;
  int32_t  s32;
  uint32_t a = 1, b = 2;

  u8  = (uint8_t)(u32 >> 8);  // UndCC_Valid - same category and narrower
  s32 = (int32_t)(a - b);     // UndCC_Violation - unsigned composite cast to signed
}

// A cast applies to the unary expression that follows it, which may be a
// complement, a unary minus or a conditional
void unaryAndConditionalOperands(uint32_t u32, uint16_t u16a, uint16_t u16b, int flag) {
  uint8_t  u8;
  int16_t  s16 = 0;
  int32_t  s32;
  uint32_t r;

  u8  = (uint8_t)~u32;                         // UndCC_Valid - same category and narrower
  s32 = (int32_t)~s16;                         // UndCC_Valid - a signed complement is already 32 bit
  r   = (uint32_t)(flag ? u16a : u16b);        // UndCC_Valid - neither operand is composite
  r   = (uint32_t)-u16a;                       // UndCC_Valid - the operand is not composite

  s32 = (int32_t)~u32;                         // UndCC_Violation - different category
  r   = (uint32_t)~u16a;                       // UndCC_Violation - cast to a wider type
  r   = (uint32_t)-(u16a + u16b);              // UndCC_Violation - unary minus of a composite
  r   = (uint32_t)(flag ? u16a + u16b : u16a); // UndCC_Violation - conditional with a composite operand
}
