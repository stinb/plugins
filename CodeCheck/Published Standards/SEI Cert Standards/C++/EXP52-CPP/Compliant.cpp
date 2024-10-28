#include <iostream>

void f()
{
    int a = 14;
    int b = sizeof(a);
    ++a;
    std::cout << a << ", " << b << std::endl;
}