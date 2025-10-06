// MISRA C++ 2023

#include <cstdint>
#include <cassert>

class B
{
public:
    virtual int16_t f(int16_t a, int16_t b);
};
class D1 : public B
{
public:
    int16_t f(int16_t a, int16_t b) // UndCC_Violation(a) - 'b' unused (used in overridden function, disallowed by option)
    {
        return a;
    }
};
class D2 : public B
{
public:
    int16_t f(int16_t a,        // UndCC_Valid - 'a' is used
              int16_t) override // Rule does not apply - unnamed parameter
    {
        return a;
    }
};
class D3 : public B
{
public:
    int16_t f(int16_t a, int16_t b) override // UndCC_Valid
    {
        return a + b;
    }
};
class D4 : public B
{
public:
    int16_t f(int16_t a,                           // UndCC_Valid
              int16_t b [[maybe_unused]]) override // Rule does not apply -
    // declared [[maybe_unused]]
    {
        assert(b > 0); // assert macro may expand to nothing,
        // leaving 'b' unused.
        return a;
    }
};
void f1(int32_t i, // UndCC_Violation
        int32_t j) // UndCC_Valid - explicitly cast to void
{
    (void)j;
    auto l = [](int32_t m, // UndCC_Valid
                int32_t n) // UndCC_Violation
    {
        return m;
    };
}
template <bool b>
int32_t f2(int32_t i,                  // UndCC_Violation(Win) for f2< false >
           int32_t j [[maybe_unused]]) // Rule does not apply - [[maybe_unused]]
{
    if constexpr (b)
    {
        return i + j;
    }
    return 0;
}
void f3([[maybe_unused]] int32_t i, // UndCC_Valid
        int32_t j)                  // UndCC_Valid - explicitly cast to void
{
    (void)j;
    auto l = [](int32_t m,                  // UndCC_Valid
                [[maybe_unused]] int32_t n) // UndCC_Valid
    {
        return m;
    };
}