// $Id: A13-6-1.cpp 289436 2017-10-04 10:45:23Z michal.szczepankiewicz $
#include <cstdint>

typedef int notint;
typedef notint typedefint;

void Fn() noexcept
{
    std::uint32_t decimal1 = 3'000'000;       // UndCC_Valid
    std::uint32_t decimal2 = 4'500;           // UndCC_Valid
    std::uint32_t decimal2_1 = 4'50'0;        // UndCC_Violation
    std::uint32_t decimal2_3 = 4500;          // UndCC_Valid
    float decimal4 = 3.141'592'65;            // UndCC_Valid
    float decimal5 = 3.1'4159'265'3;          // UndCC_Violation
    std::uint32_t hex1 = 0xFF'FF'FF'FF;       // UndCC_Valid
    std::uint32_t hex1_1 = 0xFFFFFFFF;        // UndCC_Valid
    std::uint32_t hex2 = 0xFAB'1'FF'FFF;      // UndCC_Violation
    std::uint8_t binary1 = 0b1001'0011;       // UndCC_Valid
    std::uint8_t binary1_2 = 0b10010011;      // UndCC_Valid
    std::uint8_t binary2 = 0b10'00'10'01;     // UndCC_Violation
    int decimal_test1 = 3'400.001002;         // UndCC_Valid
    int decimal_test2 = 3400.001'002;         // UndCC_Valid
    int decimal_test3 = 3400.001'03;          // UndCC_Valid
    typedefint decimal_test4 = 54'00'30;      // UndCC_Violation
    std::uint32_t hex_test = 0xF'EF'AF;       // UndCC_Valid
    std::uint8_t binary_test = 0b1'0010'1001; // UndCC_Valid
    char char_test = 43'34'00;                // UndCC_Violation
}
