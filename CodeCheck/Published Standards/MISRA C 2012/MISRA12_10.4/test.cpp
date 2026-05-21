#include <stdint.h>
int main() {
  enum enuma { A1, A2, A3 } ena;
  enum enumb { B1, B2, B3 } enb;
  uint8_t u8a;
  int8_t s8a;
  uint16_t u16b;
  unsigned int a;
  unsigned int res = 7U;
  int sx;
  char cha;
  char *p = 0;
  ena > A1; // UndCC_Valid
  u8a + u16b; // UndCC_Valid
  cha += u8a; // UndCC_Valid
  cha -= u8a; // UndCC_Valid

  // Literal operands with matching essential type are OK
  sx == 10;      // UndCC_Valid signed and signed-literal
  a == 10U;      // UndCC_Valid unsigned and unsigned-literal
  res += 1U;     // UndCC_Valid unsigned and unsigned-literal

  // Pointer compared to integer literal: not flagged (common idiom)
  p == 0;        // UndCC_Valid pointer and integer literal

  s8a += u8a; /* UndCC_Violation signed and unsigned */
  u8a + 2; /* UndCC_Violation unsigned and signed-literal */
  res == 10; /* UndCC_Violation unsigned and signed-literal (issue #4882) */
  a += 1; /* UndCC_Violation unsigned and signed-literal */
  enb > A1; /* UndCC_Violation enum<enumb> and enum<enuma> */
  ena == enb; /* UndCC_Violation enum<enuma> and enum<enumb> */
  u8a -= cha; /* UndCC_Violation unsigned and char */
}

