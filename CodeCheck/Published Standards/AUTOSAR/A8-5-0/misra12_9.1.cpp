// MISRA C 2012

#include <cstdint>

#define bool_t bool

void f ( bool_t b, std::uint32_t *p )
{
  if ( b )
  {
    *p = 3U;
  }
}
void g ( void )
{
  std::uint32_t u;
  f ( false, &u );
  if ( u == 3U ) // UndCC_FalseNeg Could be initialized on line 12 since its passed by reference on line 12
  {
  }
  std::int32_t x;
  x = x + 1u; // UndCC_FalseNeg would have to check for multiple statements on a single line otherwise for loops get flagged
}
