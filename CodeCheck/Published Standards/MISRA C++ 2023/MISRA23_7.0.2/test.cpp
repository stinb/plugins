#include <cstdint>
#include <iostream>

int main()
{
    bool b1 = true;
    double d1;
    int8_t s8a;
    uint8_t u8a, u8b, u8c, u8d;
    int16_t s16a;
    int32_t s32a, s32b, s32c;

    if (u8a < u8b && (u8c < u8d)) // UndCC_Valid
    {
    }

    if ((u8a < u8b) && (u8c + u8d)) // UndCC_Violation
    {
    }

    if (true && (u8c < u8d)) // UndCC_Valid
    {
    }

    if (1 && (u8c < u8d)) // UndCC_Violation
    {
    }

    if (u8a && (u8c < u8d)) // UndCC_Violation
    {
    }

    if (!0) // UndCC_Violation
    {
    }

    if (!false) // UndCC_Valid
    {
    }

    s32a = s16a ? s32b : s32c;       // UndCC_Violation
    s32a = b1 ? s32b : s32c;         // UndCC_Valid
    s32a = (s16a < 5) ? s32b : s32c; // UndCC_Valid

    int32_t fn();

    bool fb();

    while (int32_t i1 = fn()) // UndCC_Valid by exception #4
    {
    }

    if (int32_t i2 = fn()) // UndCC_Violation
    {
    }

    if (int32_t i3 = fn(); i3 != 0) // UndCC_Valid version of the above line
    {
    }

    while (std::cin) // UndCC_FalsePos, lib standard cannot be validated, valid by exception #2 - std::istream has explicit operator bool
    {
    }

    for (int32_t x = 10; x; --x) // UndCC_Violation
    {
    }

    extern int32_t *getptr();

    if (getptr()) // UndCC_Valid by exception #2 - contextual conversion from pointer to bool
    {
    }

    bool b2 = getptr();            // UndCC_Violation
    bool b3 = getptr() != nullptr; // UndCC_Valid

    if (bool b4 = fb()) // UndCC_Valid
    {
    }

    if (int32_t i = fn(); i != 0) // UndCC_Valid
    {
    }

    if (int32_t i = fn(); i) // UndCC_Violation - condition has type of 'i'
    {
    }

    if (int32_t i = fn()) // UndCC_Violation
    {
    }

    if (u8a) // UndCC_Violation
    {
    }
}

class C
{
    int32_t x;

public:
    explicit operator bool() const { return x < 0; }
};

void foo(C c)
{
    bool b1 = static_cast<bool>(4); // UndCC_Violation
    bool b2 = static_cast<bool>(c); // UndCC_Valid by exception #1
    if (c)                          // UndCC_Valid by exception #2 - contextual
    {                               // conversion from 'C' to bool
    }
}
