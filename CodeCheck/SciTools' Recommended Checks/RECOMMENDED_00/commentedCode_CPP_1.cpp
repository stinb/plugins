// A2-7-2

#include <cstdint>
void Fn1() noexcept
{
    std::int32_t i = 0;
    // /*
    //  *  ++i; /* UndCC_Violation incrementing the variable i */
    //  */  // C-style comments nesting is not supported,
    //  compilation error
    for (; i < 10; ++i)
    {
        // ...
    }
}
void Fn2() noexcept
{
    std::int32_t i = 0;
    // ++i;  // Incrementing the variable i // UndCC_Violation - code should not
    // be commented-out
    for (; i < 10; ++i)
    {
        // ...
    }
}
void Fn3() noexcept
{
    std::int32_t i = 0;
    ++i;  // Incrementing the variable i using ++i syntax // UndCC_Valid - code
          // is not commented-out, but ++i occurs in a
          // comment too
    for (; i < 10; ++i)
    {
        // ...
    }
}
