#include <cstdint>

int32_t fn1()
{
    // No return
} // UndCC_Violation

int32_t fn2(int32_t x)
{
    if (x > 100)
    {
        throw 42; // Exiting via an exception
    }
    else
    {
        return x; // Value returned on other path
    }
} // UndCC_Valid
