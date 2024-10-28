#include "Poly.h"
#include <iostream>

// ... definitions for S, T, globI, globD ...

void f(const S *someSes, std::size_t count)
{
    for (const S *end = someSes + count; someSes != end; ++someSes)
    {
        std::cout << someSes->i << std::endl;
    }
}

int main()
{
    T test[5];
    f(test, 5); // UndCC_Violation
}