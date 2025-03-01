// $Id: A8-5-3.cpp 289436 2017-10-04 10:45:23Z michal.szczepankiewicz $
#include <cstdint>
#include <initializer_list>
void Fn() noexcept
{
    auto x1(10); // UndCC_Valid - the auto-declared variable is of type int, but
    // not compliant with A8-5-2.
    auto x2{10}; // UndCC_Violation - according to C++14 standard the
    // auto-declared variable is of type std::initializer_list.
    // However, it can behave differently on different compilers.
    auto x3 = 10; // UndCC_Valid - the auto-declared variable is of type int, but
    
    auto x4 = {10}; // UndCC_Violation - the auto-declared variable is of type
    // std::initializer_list, non-compliant with A8-5-2.
    std::int8_t x5{10}; // UndCC_Valid
}
