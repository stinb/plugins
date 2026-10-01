#include <cstdint>

struct S
{
  S &operator=( const S & );
  bool operator!( ) const;
};

extern std::int32_t f( );
extern std::int32_t a[ 10 ];

void g( bool bool_a, bool bool_b, std::int32_t x, std::int32_t y,
        std::int32_t b, std::int32_t c, S s1, S s2 )
{
  x = y;                                /* UndCC_Valid */
  a[ x ] = a[ x = y ];                  /* UndCC_Violation */

  if ( bool_a = bool_b )                /* UndCC_Violation */
  {
  }

  /* A declaration initializes rather than assigns, so the rule does not
     apply. C cannot express this, which is why only C++ reaches it. */
  if ( std::uint8_t i = y )             /* UndCC_Valid */
  {
    ( void ) i;
  }

  if ( ( 0u == 0u ) || ( bool_a = bool_b ) )   /* UndCC_Violation */
  {
  }

  if ( ( x = f( ) ) != 0 )              /* UndCC_Violation */
  {
  }

  a[ b += c ] = a[ b ];                 /* UndCC_Violation */
  x = b = c = 0;                        /* UndCC_Violation */

  a[ b *= c ] = a[ b ];                 /* UndCC_FalseNeg - only = += -= are read */

  /* The rule does not apply to an assignment in an unevaluated operand. */
  ( void ) sizeof( x = y );             /* UndCC_Valid */
  using T = decltype( x = y );          /* UndCC_Valid */
  ( void ) noexcept( x = y );           /* UndCC_Valid */
  T r = x;
  ( void ) r;

  /* The same declaration form in the other two conditions. */
  while ( std::int32_t i = f( ) )       /* UndCC_Valid */
  {
    ( void ) i;
    break;
  }

  for ( std::int32_t j = 0; j < 2; ++j )  /* UndCC_Valid */
  {
  }

  std::int32_t m = ( x = y );           /* UndCC_Violation - the inner value is used */
  ( void ) m;

  ( void ) !( s1 = s2 );                /* UndCC_FalseNeg - overloaded operator= */
}
