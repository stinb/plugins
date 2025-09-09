#include <functional>

void f()
{
    auto l = [](const int &j)
    { return j; };
    std::function<const int &(const int &)> fn(l); // UndCC_Violation(win, mac)

    int i = 42;
    int j = fn(i);
}