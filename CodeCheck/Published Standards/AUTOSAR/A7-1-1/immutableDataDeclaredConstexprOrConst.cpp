//% $Id: A7-1-1.cpp 289436 2017-10-04 10:45:23Z michal.szczepankiewicz $
#include <cstdint>
#include <limits>
void Fn()
{
    const std::int16_t x1 = 5;     // UndCC_Valid
    constexpr std::int16_t x2 = 5; // UndCC_Valid
    std::int16_t x3 = 5;           // UndCC_Violation - x3 is not modified but not declared as
                                   // constant (const or constexpr)
    int arr[x1];
}
