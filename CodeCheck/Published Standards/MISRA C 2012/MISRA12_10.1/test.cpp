int main() {
  enum enuma { a1, a2, a3 } ena, enb; /* Essentially enum<enuma> */
  enum { K1 = 1, K2 = 2 }; /* Essentially signed */
  float f32a;
  float f32b;

  char cha;
  int chb;
  signed int s8a;
  signed int s8b;
  signed int s16b;
  unsigned int u8a;
  unsigned int u8b;
  unsigned int u16b;
  bool bla;
  bool blb;


  int a[bla]; // UndCC_Violation Rationale 3
  cha && bla; /* UndCC_Violation Rationale 2 - char type used as a Boolean value */
  ena ? a1 : a2; /* UndCC_Violation Rationale 2 - enum type used as a Boolean value */
  s8a && bla; /* UndCC_Violation Rationale 2 - signed type used as a Boolean value */
  u8a ? a1 : a2; /* UndCC_Violation Rationale 2 - unsigned type used as a Boolean value */
  f32a && bla; /* UndCC_Violation Rationale 2 - floating type used as a Boolean value */
  bla * blb; /* UndCC_Violation Rationale 3 - Boolean used as a numeric value */
  bla > blb ;/* UndCC_Violation Rationale 3 - Boolean used as a numeric value */
  cha & chb; /* UndCC_Violation Rationale 4 - char type used as a numeric value */
  cha << 1; /* UndCC_Violation Rationale 4 - char type used as a numeric value */
  ena * a1; /* UndCC_Violation Rationale 5 - enum type used in arithmetic operation */
  s8a & 2; /* UndCC_Violation Rationale 6 - bitwise operation on signed type */
  50 << 3U; /* UndCC_False_Neg Can't check literals - Rationale 6 - shift operation on signed type */
  u8a << s8a; /* UndCC_Violation Rationale 7 - shift magnitude uses signed type */
  u8a << -1; /* UndCC_False_Neg Can't check literals - Rationale 7 - shift magnitude uses signed type */
    -u8a; /* UndCC_Violation Rationale 8 - unary minus on unsigned type */



  bla && blb; // UndCC_Valid
  bla ? u8a : u8b; // UndCC_Valid
  cha - chb; // UndCC_Valid
  cha > chb; // UndCC_Valid
  ena > a1; // UndCC_Valid
  K1 * s8a; /* UndCC_Valid as K1 from anonymous enum */
  s8a + s16b; // UndCC_Valid
  -( s8a ) * s8b; // UndCC_Valid
  s8a > 0; // UndCC_Valid
  --s16b; // UndCC_Valid
  u8a + u16b; // UndCC_Valid
  u8a & 2U; // UndCC_Valid
  u8a > 0U; // UndCC_Valid
  u8a << 2U; // UndCC_Valid
  u8a << 1; /* UndCC_Valid by exception */
  f32a + f32b; // UndCC_Valid
  f32a > 0.0; // UndCC_Valid
}

