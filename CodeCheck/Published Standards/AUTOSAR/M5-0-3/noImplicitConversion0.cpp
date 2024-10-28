#include <cstdint>

void f ( )
{
    int32_t s32;
    int8_t s8;
    s32 = s8 + s8;                             // UndCC_Violation
    s32 = static_cast < int32_t > ( s8 ) + s8; // UndCC_Valid
    s32 = s32 + s8;                            // UndCC_Valid
}
