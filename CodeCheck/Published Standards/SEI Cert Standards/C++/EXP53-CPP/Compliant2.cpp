#include <iostream>

void f()
{
    int *i = new int(12);
    std::cout << i << ", " << *i;
}