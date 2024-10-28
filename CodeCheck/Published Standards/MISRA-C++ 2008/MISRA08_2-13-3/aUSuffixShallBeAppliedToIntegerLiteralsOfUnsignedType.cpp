template <typename T>
void f ( T );
template <>
void f < unsigned int > ( unsigned int );
template <>
void f < int > ( int );
void b ( )
{
  unsigned int u16a = 0u; // UndCC_Valid
  f ( 0x8000 ); // UndCC_Violation on a 16-bit platform.
  f ( 0x8000l ); // Should not throw an error
  u16a = u16a + 0x8000; // UndCC_Violation as context is unsigned.
}
