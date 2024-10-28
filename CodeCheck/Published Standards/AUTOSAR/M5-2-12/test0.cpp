// MISRA C++ 2008

#include <cstdint>

void f1( int32_t p[ 10 ] );
void f2( int32_t *p );
void f3( int32_t ( &p )[ 10 ] );

void b ()
{
    int32_t a[ 10 ];
    f1( a ); // UndCC_Violation(strict,whitelist) - Dimension "10" lost due to array to pointer conversion.
    f2( a ); // UndCC_Violation(strict,whitelist) - Dimension "10" lost due to array to pointer conversion.
    f3( a ); // UndCC_Valid - Dimension preserved.
}
