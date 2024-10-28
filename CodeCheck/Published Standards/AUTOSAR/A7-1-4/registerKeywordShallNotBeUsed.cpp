// $Id: A7-1-4.cpp 289448 2017-10-04 11:11:03Z michal.szczepankiewicz $
#include <cstdint>

std::int32_t F1(register std::int16_t number) noexcept // UndCC_Violation
{
    return ((number * number) + number);
}

void F2(std::int16_t number) noexcept // UndCC_Valid
{
    register std::int8_t x = 10;      // UndCC_Violation
    std::int32_t result = F1(number); // UndCC_Valid
    // ...
}
