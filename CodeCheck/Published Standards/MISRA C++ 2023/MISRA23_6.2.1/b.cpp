#include <cstdint>

// File b.cpp
struct S1 // UndCC_Violation - definitions of S1 are not the same
{
    int64_t i;
};
struct S2 // UndCC_Violation - definitions of S2 are not the same
{
    int32_t i;
    int32_t j;
};