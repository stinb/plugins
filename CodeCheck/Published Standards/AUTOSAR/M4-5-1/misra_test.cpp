#include <cstdint>

void foo()
{
    bool b1 = true;
    bool b2 = false;
    double d1;
    int8_t s8a;

    if (b1 & b2) // UndCC_Violation - b1 and b2 converted to int
    {
    }
    if (b1 < b2) // UndCC_Violation - b1 and b2 converted to int
    {
    }
    if (~b1) // UndCC_Violation - b1 converted to int
    {
    }
    if (b1 ^ b2) // UndCC_Violation - b1 and b2 converted to int
    {
    }
    if (b1 == 0) // UndCC_Violation - b1 converted to int
    {
    }
    double result = d1 * b1; // UndCC_Violation - b1 converted to double

    s8a = static_cast<int8_t>(b1); // UndCC_Violation(1) - b1 converted to int8_t

    if (b1 == false) // UndCC_Valid - Boolean operands to equality
    {
    }
    if (b1 == b2) // UndCC_Valid - Boolean operands to equality
    {
    }
    if (b1 != b2) // UndCC_Valid - Boolean operands to equality
    {
    }
    if (b1 && b2) // UndCC_Valid - no conversion from bool
    {
    }
    if (!b1) // UndCC_Valid - no conversion from bool
    {
    }
    s8a = b1 ? 3 : 7; // UndCC_Valid - no conversion from bool

    void f(int32_t n);
    bool b;
    f(b);         // UndCC_Violation(1) - b converted to int32_t
    f(b ? 1 : 0); // UndCC_Valid - no conversion from bool

    switch (b) // UndCC_Violation(1) - b converted to int
    {
    }

    struct A
    {
        explicit A(bool);
    };

    A anA{true};    // UndCC_Valid - constructor
    anA = A{false}; // UndCC_Valid - explicit cast calls constructor
}
