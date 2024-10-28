#include <iostream>

void f(int i)
{
    std::cout << i++ << i << std::endl; // UndCC_Violation
}