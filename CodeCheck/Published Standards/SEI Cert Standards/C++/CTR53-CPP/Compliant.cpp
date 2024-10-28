#include <algorithm>
#include <iostream>
#include <vector>

void f(const std::vector<int> &c)
{
    std::for_each(c.begin(), c.end(), [](int i)
                  { std::cout << i; });
}