#include <iostream>
#include <limits>

void f()
{
    int i;
    std::cin >> i;
    if (std::cin.fail())
    {
        // Handle failure to convert the value.
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), ' ');
    }

    int j;
    std::cin >> j;
    if (std::cin.fail())
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), ' ');
    }

    // ...
}