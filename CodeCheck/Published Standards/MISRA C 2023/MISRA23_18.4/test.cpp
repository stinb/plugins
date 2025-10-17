#include <cstdint>
void fn1 ( void )
{
  uint8_t a[ 10 ];
  uint8_t *ptr;
  uint8_t index = 0U;
  index = index + 1U; /* UndCC_Valid - rule only applies to pointers */
  a[ index ] = 0U; /* UndCC_Valid */
  ptr = &a[ 5 ]; /* UndCC_Valid */
  ptr = a;
  ptr++; /* UndCC_Valid - increment operator not + */
  *( ptr + 5 ) = 0U; /* UndCC_Violation */
  ptr[ 5 ] = 0U; /* UndCC_Valid */
}
void fn2 ( void )
{
  uint8_t array_2_2[ 2 ][ 2 ] = { { 1U, 2U }, { 4U, 5U } };
  uint8_t i = 0U;
  uint8_t j = 0U;
  uint8_t sum = 0U;
  for ( i = 0U; i < 2U; i++ )
  {
    uint8_t *row = array_2_2[ i ];
    for ( j = 0U; j < 2U; j++ )
    {
      sum += row[ j ]; /* UndCC_Valid */
    }
  }
}
void fn3 ( uint8_t *p1, uint8_t p2[ ] )
{
  p1++; /* UndCC_Valid */
  p1 = p1 + 5; /* UndCC_Violation */
  p1[ 5 ] = 0U; /* UndCC_Valid */
  p2++; /* UndCC_Valid */
  p2 = p2 + 3; /* UndCC_Violation */
  p2[ 3 ] = 0U; /* UndCC_Valid */
}
uint8_t a1[ 16 ];
uint8_t a2[ 16 ];
uint8_t data = 0U;
void fn4 ( void )
{
  fn3 ( a1, a2 );
  fn3 ( &data, &a2[ 4 ] );
}

