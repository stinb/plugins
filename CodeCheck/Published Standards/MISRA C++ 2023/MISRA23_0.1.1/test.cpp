#include <cstdint>
#include <string>

extern std::int32_t f( std::int32_t );
extern void use( std::int32_t );
extern std::string make( );
extern void useStr( const std::string & );

void overwritten( )
{
  std::int32_t i = f( 1 );              /* UndCC_Violation - overwritten first */
  i = 2;
  use( i );
}

void never_read( )
{
  std::int32_t i = f( 1 );              /* UndCC_Violation - destroyed unobserved */
}

void read_then_written( )
{
  std::int32_t i = f( 1 );              /* UndCC_Valid - observed below */
  use( i );
  i = 2;                                /* UndCC_Violation - destroyed unobserved */
}

void properly_used( )
{
  std::int32_t i = f( 1 );              /* UndCC_Valid */
  use( i );
}

void container( )
{
  std::string s = make( );              /* UndCC_FalseNeg - containers are not tracked */
  s = make( );
  useStr( s );
}

void through_reference( )
{
  std::int32_t local = 0;
  std::int32_t &ref = local;
  ref = 1;                              /* UndCC_FalseNeg - written through a reference */
  ref = 2;
  use( local );
}

void array_element( )
{
  std::int32_t arr[ 2 ] = { 0, 0 };
  arr[ 0 ] = 1;                         /* UndCC_FalseNeg - subobjects are not tracked */
  arr[ 0 ] = 2;
  use( arr[ 0 ] );
}

void dead_nested( )
{
  std::int32_t a;
  std::int32_t b;

  /* The value of a = f( 1 ) is used by the enclosing assignment, but a
     itself is never read. */
  b = a = f( 1 );                       /* UndCC_Violation */
  use( b );
}
