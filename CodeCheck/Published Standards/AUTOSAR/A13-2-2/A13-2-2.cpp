// $Id: A13-2-2.cpp 271687 2017-03-23 08:57:35Z piotr.tanski $
#include <cstdint>

class A
{
};

A operator+(A const &, A const &) noexcept // UndCC_Valid
{
    return A{};
}
std::int32_t operator/(A const &, A const &) noexcept // UndCC_Valid
{
    return 0;
}
A operator&(A const &, A const &) noexcept // UndCC_Valid
{
    return A{};
}
const A operator-(A const &, std::int32_t) noexcept // UndCC_Violation
{
    return A{};
}
A *operator|(A const &, A const &) noexcept // UndCC_Violation
{
    return new A{};
}