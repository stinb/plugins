#include <cstdint>

int main() {
    uint8_t port = 0x5aU;
    uint8_t result_8;
    uint16_t result_16;
    uint16_t mode;
    result_8 = ( ~port ) >> 4; // UndCC_Violation

    result_8 = ( static_cast< uint8_t > (~port) ) >> 4 ; // UndCC_Valid

    result_16 = ( ( port << 4 ) & mode ) >> 6; // UndCC_Violation

    result_16 =
        ( static_cast < uint16_t > ( static_cast< uint16_t > ( port ) << 4 )
        & mode ) >> 6; // UndCC_Valid

    uint16_t port_16 = static_cast< uint16_t > ( port );
    uint16_t port_shifted = static_cast< uint16_t > ( port_16 << 4 );
    result_16 = ( port_shifted & mode ) >> 6; // UndCC_Valid
}
