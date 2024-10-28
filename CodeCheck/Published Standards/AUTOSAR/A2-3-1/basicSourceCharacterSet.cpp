#include <cstdint>

void Fn() noexcept
{
    std::int32_t sum = 0;            // UndCC_Valid
    // std::int32_t Â£_value = 10;   // UndCC_Violation
    // sum += Â£_value;              // UndCC_Violation
    // Variable sum stores Â£ pounds // UndCC_Violation
}
