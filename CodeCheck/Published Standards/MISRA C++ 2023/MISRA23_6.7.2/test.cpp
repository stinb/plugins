#include <cstdint>

int32_t foo();
int32_t i1{foo()};    // UndCC_Violation
const int32_t i2{i1}; // UndCC_Violation - dynamic initialization

namespace
{
    int32_t i3{0}; // UndCC_Violation
    constexpr int32_t bar()
    {
        return 42;
    }
    constexpr int32_t i4{bar()}; // Rule does not apply - constexpr
    const int32_t SIZE{100};     // Rule does not apply - const without dynamic initialization
}

struct ComplexInit
{
    ComplexInit();
};

const ComplexInit c1{}; // UndCC_Violation - dynamic initialization

class StaticMember
{
    int32_t x{5}; // Rule does not apply
    static int32_t numInstances;
};

int32_t StaticMember::numInstances = 0; // UndCC_Violation

constexpr auto add = // Rule does not apply - add is const
    [](auto x, auto y)
{ return x + y; };