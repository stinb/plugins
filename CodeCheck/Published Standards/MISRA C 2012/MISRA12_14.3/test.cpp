#include <cstdint>
#include <type_traits>

extern bool get( );
extern std::uint16_t value( );

template< typename T >
std::int32_t dispatch( T v )
{
  /* A constexpr if is evaluated at compile time and the rule excludes it. */
  if constexpr ( std::is_integral_v< T > )
  {
    return static_cast< std::int32_t >( v );
  }
  else
  {
    return 0;
  }
}

void f( )
{
  std::int8_t s8a = 0;

  if ( true )                           /* UndCC_Violation */
  {
  }

  if ( 2 > 3 )                          /* UndCC_Violation */
  {
  }

  s8a = ( 1 > 2 ) ? 0 : 1;              /* UndCC_Violation - the first operand */

  switch ( 3 )                          /* UndCC_Violation */
  {
    default:
      break;
  }

  if ( get( ) )                         /* UndCC_Valid */
  {
  }

  if ( value( ) == 4u )                 /* UndCC_Valid */
  {
  }

  while ( true )                        /* UndCC_Valid - exception 1 */
  {
    break;
  }

  do
  {
  }
  while ( false );                      /* UndCC_Valid - exception 2 */

  if ( ( s8a < 10 ) && ( s8a > 20 ) )   /* UndCC_FalseNeg - operands are not judged */
  {
  }

  ( void ) dispatch( 1 );
  ( void ) dispatch( 1.0 );
}
