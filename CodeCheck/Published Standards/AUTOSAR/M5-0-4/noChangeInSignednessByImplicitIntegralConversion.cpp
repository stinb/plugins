#include <cstdint>

/*
Some signed to unsigned conversions may lead to implementation-defined behaviour. This
behaviour may not be consistent with developer expectations.
*/

typedef int8_t myInt;
typedef myInt osbcureInt;

int8_t f2( uint8_t u8temp )
{
    return static_cast< int8_t > ( u8temp );
}

void f()
{
    int8_t s8;
    uint8_t u8;
    osbcureInt s9;
    double d10;

    s8 = u8;                                  // UndCC_Violation
    u8 = s8 + u8;                             // UndCC_Violation
    u8 = static_cast< uint8_t > ( s8 ) + u8;  // UndCC_Valid
    s8 = f2(u8) + s9;                         // Compliant
    s8 = s9;                                  // Compliant
    u8 = s9;                                  // UndCC_Violation
}
