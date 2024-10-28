#include <cstdint>

std::uint32_t Fn() noexcept
{
    using LocalUIntPtr1 = std::uint32_t *; // UndCC_Violation
    return 0U;
}

std::uint32_t Fn2() noexcept
{
    using LocalUIntPtr2 = std::uint32_t *; // UndCC_Valid
    LocalUIntPtr2 hey;
    return *hey;
}
