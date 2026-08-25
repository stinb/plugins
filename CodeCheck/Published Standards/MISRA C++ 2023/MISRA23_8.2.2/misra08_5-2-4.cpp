#include <cstdint>
class A
{
public:
    explicit A( int32_t );
};
int32_t g ( )
{
    return 7;
}
void f ( )
{
    A const a1 = A( 10 );            // UndCC_Valid - explicit constructor call
    A * a2 = ( A* )( &a1 );          // UndCC_Violation - C-style cast
    A * a3 = const_cast<A*>( &a1 );  // UndCC_Valid - not a C-style cast
    (void)g ( );                     // UndCC_Valid - cast to void, by exception
}
