#include <cstdint>


uint8_t u8a;
uint16_t u16a;


int main() {
    // MISRA04_12.8 & MISRA08_5-8-1
    u8a = (uint8_t) ( u8a << 7 );                 // UNDCC_Valid
    u8a = (uint8_t) ( u8a << 9 );                 // UNDCC_Violation
    u16a = (uint16_t) ( (uint16_t) u8a << 9 );    // UNDCC_Valid


    // MISRA12_12.2
    1u << 10u;                 // UNDCC_Violation - Valid if literal int is not treated as char
    ( uint16_t ) 1u << 10u;    // UNDCC_Valid
    1UL << 10u;                // UNDCC_Valid


    // Additional
    const int8_t x = 1;
    const int8_t y = -2;
    x << y;                 // UNDCC_Violation
}


// A typedef of a fixed width type keeps that width
typedef uint32_t uint_t;

void typedefChain(uint_t result) {
    result << 16;           // UNDCC_Valid
    result << 32;           // UNDCC_Violation
}
