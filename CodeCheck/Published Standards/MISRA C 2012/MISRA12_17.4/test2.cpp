// AUTOSAR C++ 2014

#include <cstdint>
#include <stdexcept>
std::int32_t F1() noexcept
{
} // UndCC_Violation
std::int32_t F2(std::int32_t x) noexcept(false)
{
    if (x > 100)
    {
        throw std::logic_error("Logic Error");
    }

    return x;
} // UndCC_Valid by exception
std::int32_t F3(std::int32_t x, std::int32_t y)
{
    if (x > 100 || y > 100)
    {
        throw std::logic_error("Logic Error");
    }
    if (y > x)
    {
        return (y - x);
    }
    return (x - y);
} // UndCC_Valid by exception
