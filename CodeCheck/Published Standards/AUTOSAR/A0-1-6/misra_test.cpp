#include <cstdint>

int16_t f1()
{
    using T1 = int16_t;                  // UndCC_Violation
    using T2 [[maybe_unused]] = int32_t; // UndCC_Valid by exception #1
    return 67;
}
namespace
{
    struct A1 // UndCC_FalsePos(Win, Mac) - no reference to `A1 a;`
    {
        A1 f();
    };
    struct A2 // UndCC_Violation
    {
        A2 f();
    };
    struct A2;                   // Not a use of A2
    A2 A2::f() { return *this; } // Not a use of A2
    template <typename T>        // UndCC_Valid by exception #2
    void foo()
    {
        A1 a;  // Use of A1
        a.f(); // - even if foo is not instantiated
    }
}
template <bool cond>
inline auto foo()
{
    struct res // UndCC_Valid
    {
        int32_t i;
    };
    if constexpr (cond)
    {
        return 42;
    }
    else
    {
        return res{42}; // res is utilized, even if cond is true
    }
}
template <typename>
int32_t bar()
{
    return 42;
}
int32_t f2()
{
    return bar<struct P>(); // UndCC_Valid - P is used
}
namespace
{
    template <typename>
    struct C1 // UndCC_Violation - C1 only utilized in its definition
    {
    };
    template <>
    struct C1<int32_t> // UndCC_Valid by exception #3
    {
        void mbr()
        {
            C1<char> cc;
        }
    };
}
namespace
{
    template <typename>
    struct C2 // UndCC_Valid - C2< float > used
    {
    };
    template <>
    struct C2<int32_t>; // UndCC_Valid by exception #3
    C2<float> cf;       // Use of C2
}
namespace
{
    static union // UndCC_Violation
    {
        int32_t i1;
        int32_t j1;
    };
    static union // UndCC_Valid
    {
        int32_t i2;
        int32_t j2;
    };
}
void f3()
{
    ++i2; // Uses the anonymous union holding i2
}
namespace
{
    void f4()
    {
        [](auto) {}; // UndCC_Valid - closure type is always used
    }
}
