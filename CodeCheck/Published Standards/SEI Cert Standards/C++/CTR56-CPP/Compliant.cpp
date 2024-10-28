#include "Poly.h"
#include <iostream>

// ... definitions for S, T, globI, globD ...

void f(const S *const *someSes, std::size_t count)
{
    for (const S *const *end = someSes + count; someSes != end; ++someSes)
    {
        std::cout << (*someSes)->i << std::endl;
    }
}

int main()
{
    S *test[] = {new T, new T, new T, new T, new T};
    f(test, 5);
    for (auto v : test)
    {
        delete v;
    }
}