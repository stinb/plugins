#include <cstdint>

void fn()
{
    for (int32_t i = 0; i != 10; ++i)
    {
        if ((i % 2) == 0)
        {
            continue; // UndCC_Valid
        }
        // ...
    }
    int32_t j = -1;
    for (int32_t i = 0; i != 10 && j != i; ++i)
    {
        if ((i % 2) == 0)
        {
            continue; // UndCC_Violation - loop is not well-formed (M6-5-1)
        }
        // ...
        ++j;
    }
    for (int32_t i = 1; i != 10; i += 2)
    {
        if ((i % 2) == 0)
        {
            continue; // UndCC_Violation - loop is not well-formed (M6-5-2)
        }
    }
    for (int32_t i = 1; i != 10; i++)
    {
        i = i * 2;
        if ((i % 2) == 0)
        {
            continue; // UndCC_Violation - loop is not well-formed (M6-5-3)
        }
    }
}
