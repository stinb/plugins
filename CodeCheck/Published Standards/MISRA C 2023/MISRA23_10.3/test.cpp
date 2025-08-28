#include <cstdint>
#include <complex.h>
int main() {
  enum enuma { A1, A2, A3 } ena;
  enum enumb { B1, B2, B3 } enb;
  enum { K1=1, K2=128 };
  uint8_t u8a = 0; /* By exception */
  int8_t s8a = 0;
  uint8_t * pu8a;
  uint8_t* pu8b;
  uint8_t u8b = 0;
  uint8_t u8c = 0;
  uint8_t u8d = 0;
  uint16_t u16a;
  uint32_t u32a;
  bool flag = ( bool ) 0;
  bool set = true; /* true is essentially Boolean */
  bool get = ( u8b > u8c );
  char cha = 'a';
  ena = A1;
  s8a = K1; /* Constant value fits */
  u8a = 2; /* By exception */
  u8a = 2 * 24; /* By exception */
  cha += 1; /* cha = cha + 1 assigns character to character */
  pu8a = pu8b; /* Same essential type */
  u8a = u8b + u8c + u8d; /* Same essential type */
  u8a = ( uint8_t ) s8a; /* Cast gives same essential type */
  u32a = u16a; /* Assignment to a wider essential type */
  u32a = 2U + 125U; /* Assignment to a wider essential type */
  //use_uint16 ( u8a ); /* Assignment to a wider essential type */
  //use_uint16 ( u8a + u16b ); /* Assignment to same essential type */
  float _Complex cf32a;
  float f32a;
  
  cf32a = f32a; /* By exception 4 */
  //cf64a = f64a; /* By exception 4 */
  
  uint8_t u8f = f32a; // UndCC_FalseNeg
  bool bla = s8a; // UndCC_Violation
  char cha1 = s8a; // UndCC_Violation
  u8a = cha; // UndCC_Violation
  u8b = u8a - s8a; // UndCC_Violation
  u8c += cha; // UndCC_Violation
  //use_uint32 ( s32a ); /* signed and unsigned */
  //f32a = cf32a; /* real floating and complex floating */
}
