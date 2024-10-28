#include <iostream>

void f2()
{
    int x = 14;
    int y = sizeof(x+=2); // UndCC_Violation
    std::cout << x << ", " << y << std::endl;
}
