#include <cstdint>

void CreateRectangle( uint32_t Width, uint32_t Height ); // UNDCC_Violation
void fn1( int32_t a );
void fn2( int32_t );
void fn1( int32_t b ) // UNDCC_Violation
{
}
void fn2( int32_t b ) // UNDCC_Valid
{
}
