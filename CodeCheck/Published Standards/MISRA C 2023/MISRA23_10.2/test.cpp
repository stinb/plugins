int main() {
  float f32a;
  enum enuma { a1, a2, a3 } ena, enb; /* Essentially enum<enuma> */
  
  char cha;
  int chb;
  signed int s8a;
  signed int s8b;
  signed int s16a;
  unsigned int u8a;
  unsigned int u8b;
  unsigned int u16b;
  bool bla;
  bool blb;
  
  '0' + u8a; // UndCC_Valid
  s8a + '0'; // UndCC_Valid
  cha - '0'; // UndCC_Valid
  '0' - s8a; // UndCC_Valid
  s16a - 'a'; // UndCC_Violation
  '0' + f32a; // UndCC_Violation
  cha + ':'; // UndCC_Violation
  cha - ena; // UndCC_Violation
}
