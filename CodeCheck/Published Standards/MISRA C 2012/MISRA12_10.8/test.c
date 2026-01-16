#include <stdint.h>

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

