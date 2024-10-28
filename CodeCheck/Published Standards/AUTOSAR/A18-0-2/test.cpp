// AUTOSAR C++ 2014

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>

std::int32_t F1(const char* str) noexcept
{
    return atoi(str); // UndCC_Violation - undefined behavior if str can not
                      // be converted
}

std::int32_t F2(std::string const& str) noexcept(false)
{
    return std::stoi(str); // UndCC_Valid - throws a std::invalid_argument
                           // exception if str can not be converted
}

std::uint16_t ReadFromStdin1() // UndCC_Violation
{
    std::uint16_t a;
    std::cin >> a; // no error detection
    return a;
}

std::uint16_t ReadFromStdin2() // UndCC_Violation(Mac)
{
    std::uint16_t a;

    std::cin.clear(); // clear all flags
    std::cin >> a;
    if (std::cin.fail())
    {
        throw std::runtime_error{"unable to read an integer"};
    }
    std::cin.clear(); // clear all flags for subsequent operations
    return a;
}
