#include <iostream>

void f()
{
    int i, j;

    std::cin.exceptions(std::istream::failbit | std::istream::badbit);
    try
    {
        std::cin >> i >> j;
        // ...
    }
    catch (std::istream::failure &E)
    {
        // Handle error
    }
}