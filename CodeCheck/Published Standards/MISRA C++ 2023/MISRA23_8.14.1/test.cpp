#include <cstdint>

uint16_t h(uint16_t y)
{
    static uint16_t temp = 0;
    temp = y + temp; // This side effect is persistent
    return temp;
}
uint16_t f(uint16_t y) // The side effects within f are not
{                      // persistent, as seen by the caller
    uint16_t temp = y;
    temp = y + 0x8080U;
    return temp;
}
void g(bool ishigh)
{
    int a, x;
    if (ishigh && (a == f(x))) // UndCC_Valid - f() has no persistent
    {                          // side effects
    }
    if (ishigh && (a == h(x))) // UndCC_Violation - h() has a persistent
    {                          // side effect
    }
}

void i()
{
    volatile uint16_t v;
    uint16_t x;
    uint16_t (*fp)(uint16_t) = &h;
    if ((x == 0u) || (v == 1u)) // UndCC_Violation - access to volatile v
    {                           // is persistent
    }
    (fp != nullptr) && (*fp)(0); // UndCC_Violation if fp points to a function
                                 // with persistent side effects
    if (fp != nullptr)
    {
        (*fp)(0); // UndCC_Valid version of the above
    }
}
