// MISRA C 2012 & 2023

void test()
{
  // Assumes 16-bit int and 32-bit long
  32767;   // UndCC_Valid     - signed int
  0x7fff;  // UndCC_Valid     - signed int
  32768;   // UndCC_Valid     - signed long
  32768u;  // UndCC_Valid     - unsigned int
  0x8000;  // UndCC_Violation - unsigned int
  0x8000u; // UndCC_Valid     - unsigned int
}
