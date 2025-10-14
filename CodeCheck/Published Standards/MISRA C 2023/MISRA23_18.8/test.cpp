#include <cstdint>
void f ( void )
{
uint16_t n = 5;
typedef uint16_t Vector[ n ]; /* UndCC_Violation An array type with 5 elements */
n = 7;
Vector a1; /* An array type with 5 elements */
uint16_t a2[ n ]; /* UndCC_Violation An array type with 7 elements */
}
void f ( int16_t n )
{
  uint16_t vla[ n ]; /* UndCC_Violation - Undefined if n <= 0 */
}
void g ( void )
{
  f ( 0 ); /* Undefined */
  f ( -1 ); /* Undefined */
  f ( 10 ); /* Defined */
}
