#include <cstdint>

enum class Enum1 : int8_t // UndCC_Valid
{
    E0 = 1,
    E1 = 2,
    E2 = 4
};

enum class Enum2 // UndCC_Violation - no explicit underlying type
{
    E0 = 0,
    E1,
    E2
};

// enum class Enum3 : uint8_t // cannot represent value for E2
// {                          // Implicit value is the result of wrapping
//     E0,
//     E1 = 255,
//     E2                      // compilation error
// };

enum class Enum4 // UndCC_Violation(0) exception #1 in MISRA
{
    E0,
    E1,
    E2
};

extern "C"
{
    enum Enum5 // UndCC_Violation(0) exception #2 in MISRA
    {
        E7_0 = 0,
        E7_1,
        E7_2
    };
}
