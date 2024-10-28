// $Id: A13-2-3.cpp 271687 2017-03-23 08:57:35Z piotr.tanski $
#include <cstdint>

class A
{
};

typedef bool BOOL1;
typedef BOOL1 BOOL2;
BOOL2 operator==(A const &, A const &) noexcept // UndCC_Valid
{
  return true;
}

bool operator<(A const &, A const &) noexcept // UndCC_Valid
{
    return false;
}
bool operator!=(A const &lhs, A const &rhs) noexcept // UndCC_Valid
{
    return !(operator==(lhs, rhs));
}
std::int32_t operator>(A const &, A const &) noexcept // UndCC_Violation
{
    return -1;
}
A operator>=(A const &, A const &) noexcept // UndCC_Violation
{
    return A{};
}
const A &operator<=(A const &lhs, A const &rhs) noexcept // UndCC_Violation
{
    return lhs;
}
