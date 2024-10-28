#include <iostream>

void f()
{
    int a = 14;
    int b = sizeof(a++); // UndCC_Violation
    std::cout << a << ", " << b << std::endl;
}