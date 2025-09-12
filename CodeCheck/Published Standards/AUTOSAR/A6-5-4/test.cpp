// $Id: A6-5-4.cpp 305629 2018-01-29 13:29:25Z piotr.serwa $
#include <cstdint>
#include <vector>

int MAX = 0;

void Fn() noexcept
{
    std::int32_t y;

    for (std::int32_t x = 0, MAX = 10; x < MAX; x++) // UndCC_Violation

    {
        // ...
    }

    for (y = 0, MAX = 10; y < MAX; y++, MAX++) // UndCC_Violation - AV Rule 199
    {
        // ...
    }

    for (std::int32_t x = 0; x < MAX; x++) // UndCC_Valid
    {
        // ...
    }

    for (std::int32_t y = 0; y < 10; y++) // UndCC_Valid
    {
        // ...
    }
}

int exc()
{
    std::vector<int> values = {1, 2, 3, 4, 5};

    // Compliant: iterator is the loop counter
    for (auto it = values.begin(); it != values.end(); ++it) // UndCC_Valid
    {
        //
    }

    return 0;
}
