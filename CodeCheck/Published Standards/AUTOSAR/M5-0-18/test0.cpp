// MISRA C++ 2008

#include <cstdint>
void f1 ( )
{
    int32_t a1[ 10 ];
    int32_t a2[ 10 ];
    int32_t * p1 = a1;
    if ( p1 < a1 ) // UndCC_Valid
    {
    }
    if ( p1 < a2 ) // UndCC_Violation
    {
    }
}
