// Custom

#include <iostream>
#include <cstdint>

void output(std::uint32_t &i) // UndCC_Violation
{
    std::cout << i << '\n';
}
