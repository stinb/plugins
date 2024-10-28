#include <algorithm>
#include <iostream>
#include <vector>

void f(const std::vector<int> &c)
{
    std::vector<int>::const_iterator e;
    std::for_each(c.begin(), e, [](int i) // UndCC_Violation
                  { std::cout << i; });
}