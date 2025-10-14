#include <cstdint>

// File3.h
inline void h(int64_t i)
{
    f(i); // Nested call, UndCC_FalseNeg due to limitation
}