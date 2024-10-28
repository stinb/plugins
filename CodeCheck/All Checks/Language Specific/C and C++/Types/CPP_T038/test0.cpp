// AUTOSAR C++ 2014

#include <cstdint>
void F()
{
    std::int32_t i1 = 5;   // UndCC_Valid
    int i2 = 10;           // UndCC_Violation
    std::int64_t i3 = 250; // UndCC_Valid
    long int i4 = 50;      // UndCC_Violation
    std::int8_t i5 = 16;   // UndCC_Valid
    char i6 = 23;          // UndCC_Violation(allow_size_t,strict)
}
