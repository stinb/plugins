#include <cstdint>

extern volatile std::uint16_t v1;
extern volatile std::uint16_t v2;
extern std::int32_t func( std::int32_t, std::int32_t );
extern void h( char, char );

char g( char &c, char a )
{
  c = a;
  return c;
}

void f( std::int32_t *b )
{
  std::int32_t i = 0;
  std::int32_t x;
  char a = 0;

  x = b[ i ] + i++;                     /* UndCC_Violation */
  x = func( i++, i );                   /* UndCC_Violation */

  /* Since C++17 the right operand of an assignment is sequenced first, so
     both of these have a defined order. GCC disagrees: its -Wsequence-point
     warns on them under -std=c++14, c++17 and c++20 alike, never having
     implemented the C++17 change. The warning is not evidence of a defect
     here. */
  b[ i++ ] = b[ i++ ];                  /* UndCC_Valid */
  i = i++ + 1;                          /* UndCC_Valid */

  x = b[ i ] + i;                       /* UndCC_Valid */

  h( g( a, 'a' ), g( a, 'b' ) );        /* UndCC_FalseNeg - access through a reference */

  std::uint16_t t = v1 + v2;            /* UndCC_FalseNeg - volatile is not modelled */
  v1 = v1 & 0x80u;                      /* UndCC_Valid */

  ( void ) x;
  ( void ) t;
}
