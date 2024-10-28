#include <iostream>

void f()
{
    int *i = new int;
    std::cout << i << ", " << *i; // UndCC_Violation
}