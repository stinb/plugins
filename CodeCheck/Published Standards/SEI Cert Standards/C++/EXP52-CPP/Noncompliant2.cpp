#include <iostream>
#include <utility>

void f()
{
    int i = 0;
    decltype(i++) h = 12; // UndCC_Violation
    void a() noexcept(false);
    std::cout << i;
}