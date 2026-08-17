// Custom

#include <stdint.h>

void f()
{
  uint32_t u32a, u32b;
  uint16_t u16a, u16b;

  u32a == u16a + u16b; // UndCC_Valid
  u32a *= u16a + u16b; // UndCC_Violation
}

void constantsAndWidths(void)
{
  uint64_t u64a = 0;
  uint32_t u32a = 0;
  uint16_t u16a = 0, u16b = 0;
  uint8_t  u8a = 0;

  u64a / (1000U * 60U * 60U);  // UndCC_Valid - a constant expression is not composite
  u32a * (2048L - 1L);         // UndCC_Valid - a constant expression is not composite
  (u16a >> 8) ^ u16b;          // UndCC_Valid - operands of the same essential width
  u64a & ((1U << u8a) - 1U);   // UndCC_Violation - the composite is 8 bit, the other operand 64
}

void categoriesAndFloats(void)
{
  uint32_t u32a = 0;
  uint8_t  u8a = 0;
  int8_t   s8a = 0;
  char     cha = 'a';
  float    f32a = 0.0f, f32b = 0.0f;
  double   f64a = 0.0;

  u32a * (u8a + s8a);   // UndCC_Valid - operands of different categories take the standard type
  f64a * (f32a + f32b); // UndCC_Violation - the composite is 32 bit, the other operand 64
  u32a * (cha + 1);     // UndCC_Violation - character arithmetic stays 8 bit
}

void standardTypeOfOperands(void)
{
  char     ch = '7';
  int16_t  s16a = 0;
  int32_t  s32a = 0;
  uint32_t u32a = 0;

  s32a |= ch - '0';        // UndCC_Valid - char minus char takes the standard type
  s32a = (s16a << 2) + u32a; // UndCC_Valid - a signed shift takes the standard type
}
