#include <iostream>

void f()
{
    char buf[12];
    std::cin >> buf; // UndCC_Violation
}