#include <cstdint>

void f1(volatile int32_t i) // UndCC_Violation
{
    // use<int32_t>(i);
}

void f2(volatile int32_t *p) // UndCC_Valid - parameter is not volatile
{
    // use<int32_t *>(p);
}

void f3(int32_t *volatile p) // UndCC_Violation - parameter is volatile
{
    // use<int32_t *>(p);
}

void f4(int32_t i)
{
    volatile int32_t j = i; // UndCC_Violation
    // use<int32_t>(j);
}

volatile int32_t f5(); // UndCC_Violation

void f6()
{
    int32_t g[2] = {1, 2};
    auto volatile [a, b] = g; // UndCC_Violation
}

struct S
{
    volatile uint32_t reg; // UndCC_Valid
};

void f7(S s);  // UndCC_Valid - but unlikely to work as expected
void f8(S &s); // UndCC_Valid - preserves volatile behaviour of reg