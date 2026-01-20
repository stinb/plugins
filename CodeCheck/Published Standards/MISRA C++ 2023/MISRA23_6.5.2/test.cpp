#include <cstdint>

static void f1(); // UndCC_Violation

namespace
{
    void f2();                 // UndCC_Valid
    int32_t notExtern1;        // UndCC_Valid
    extern int32_t notExtern2; // UndCC_Violation
}
