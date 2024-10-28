#include "Poly.h"
#include <iostream>

// ... definitions for S, T, globI, globD ...

void f(const S *someSes, std::size_t count)
{
    for (std::size_t i = 0; i < count; ++i)
    {
        std::cout << someSes[i].i << std::endl;
    }
}

int main()
{
    T test[5];
    f(test, 5); // UndCC_Violation
}