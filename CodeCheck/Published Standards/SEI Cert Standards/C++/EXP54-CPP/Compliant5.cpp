#include <functional>

void f()
{
    auto l = [](const int &j)
    { return j; };
    std::function<int(const int &)> fn(l);

    int i = 42;
    int j = fn(i);
}