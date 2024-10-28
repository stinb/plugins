#include <algorithm>
#include <iostream>
#include <vector>

void f(const std::vector<int> &c)
{
    std::for_each(c.end(), c.begin(), [](int i) // UndCC_Violation
                  { std::cout << i; });
}