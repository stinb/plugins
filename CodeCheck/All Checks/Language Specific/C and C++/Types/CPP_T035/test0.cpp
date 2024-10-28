// AUTOSAR C++ 2014

#include <cstdint>
#include <iostream>
#include <string>

void output(std::uint32_t i) // UndCC_Valid - pass by value
{
    std::cout << i << '\n';
}


void output(std::string s) // UndCC_Violation - std::string is not trivially copyable
{
    std::cout << s << '\n';
}

struct A
{
    std::uint32_t v1;
    std::uint32_t v2;
};


void output(const A &a) // UndCC_Violation - A is trivially copyable and no longer than two words
{
    std::cout << a.v1 << ", " << a.v2 << '\n';
}
