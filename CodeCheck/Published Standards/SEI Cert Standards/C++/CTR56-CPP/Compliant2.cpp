#include "Poly.h"
#include <iostream>
#include <vector>

// ... definitions for S, T, globI, globD ...
template <typename Iter>
void f(Iter i, Iter e)
{
    for (; i != e; ++i)
    {
        std::cout << (*i)->i << std::endl;
    }
}

int main()
{
    std::vector<S *> test{new T, new T, new T, new T, new T};
    f(test.cbegin(), test.cend());
    for (auto v : test)
    {
        delete v;
    }
}