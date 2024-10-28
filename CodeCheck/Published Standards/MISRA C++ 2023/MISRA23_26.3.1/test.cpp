#include <vector>
#include <array>
#include <bitset>
#include <cstdint>

struct myBool
{
    bool b;
}; // Wrapper for bool

void foo() noexcept
{
    std::vector<bool> a;         // UndCC_Violation - optimized storage
    std::vector<std::uint8_t> b; // UndCC_Valid
    std::vector<myBool> c;       // UndCC_Valid
    std::array<bool, 20> d;      // Rule does not apply
    std::bitset<200> e;          // Rule does not apply - efficient storage
}
