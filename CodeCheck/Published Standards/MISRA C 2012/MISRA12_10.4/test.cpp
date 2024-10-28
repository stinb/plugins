#include <stdint.h>
int main() {
  enum enuma { A1, A2, A3 } ena;
  enum enumb { B1, B2, B3 } enb;
  uint8_t u8a;
  int8_t s8a;
  uint16_t u16b;
  unsigned int a;
  char cha;
  ena > A1; // UndCC_Valid
  u8a + u16b; // UndCC_Valid
  cha += u8a; // UndCC_Valid
  cha -= u8a; // UndCC_Valid
  
  s8a += u8a; /* UndCC_Violation signed and unsigned */
  u8a + 2; /* UndCC_FalseNeg Can't check literals unsigned and signed */
  enb > A1; /* UndCC_Violation enum<enumb> and enum<enuma> */
  ena == enb; /* UndCC_Violation enum<enuma> and enum<enumb> */
  u8a -= cha; /* UndCC_Violation unsigned and char */  
}

