#include <cstdint>

// f1.cpp
namespace n_s1
{
    static std::int32_t globalvariable = 0; // UndCC_Violation
}
static std::int32_t filevariable = 5; // UndCC_Valid - identifier not reused
static void Globalfunction(); // UndCC_Violation