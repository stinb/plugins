#include <iostream>

void f()
{
    char bufOne[12];
    char bufTwo[12];
    std::cin.width(12);
    std::cin >> bufOne; // UndCC_Violation
    std::cin >> bufTwo; // UndCC_Violation
}