#include <cstdint>

void f() {
    uint16_t uint16_a, uint16_b;
    int16_t int16_a, int16_b;

    if ( ( uint16_a & int16_b ) == 0x1234U )    // UndCC_Violation
    if ( ( uint16_a | uint16_b ) == 0x1234U )   // UndCC_Valid
    if ( ~int16_a == 0x1234U )                  // UndCC_Violation
    if ( ~uint16_a == 0x1234U )                 // UndCC_Valid
    ;
}
