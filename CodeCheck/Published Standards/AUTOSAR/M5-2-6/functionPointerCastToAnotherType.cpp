#include <cstdint>

typedef void (*type1)( );
typedef void *type2;
typedef void (*type3)( int32_t );

void f ( int32_t )
{
	reinterpret_cast< void (*)( ) >( &f );         // UndCC_Violation
	reinterpret_cast< void * >( &f );              // UndCC_Violation
	reinterpret_cast< void (*)( int32_t ) >( &f ); // UndCC_Valid
	
	reinterpret_cast< type1 >( &f );               // UndCC_Violation
	reinterpret_cast< type2 >( &f );               // UndCC_Violation
	reinterpret_cast< type3 >( &f );               // UndCC_Valid
}
