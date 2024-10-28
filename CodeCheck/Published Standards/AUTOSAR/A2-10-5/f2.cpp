#include <cstdint>

namespace n_s1
{
    static std::int32_t globalvariable = 0; // UndCC_Violation - identifier reused
    static std::int16_t modulevariable = 10; // UndCC_Valid - identifier not reused
}
namespace n_s2
{
    static std::int16_t modulevariable2 = 20;
}
static void Globalfunction();             // UndCC_Violation - identifier reused
static std::int16_t modulevariable2 = 15; // UndCC_Violation - identifier reused
