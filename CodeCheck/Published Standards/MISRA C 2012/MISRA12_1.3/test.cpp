#include <cstdint>

struct Empty { };

extern void use( std::int32_t );

void f( )
{
  const std::int32_t c = 0;
  std::int32_t m = 1;

  *( std::int32_t * ) &c = 1;           /* UndCC_Violation */

  *const_cast< std::int32_t * >( &c ) = 2;  /* UndCC_Violation */

  *( std::int32_t * ) &m = 3;           /* UndCC_Valid - m is not const */

  /* An empty class type is well defined in C++, unlike in C. */
  Empty e;
  ( void ) e;

  use( c );
}
