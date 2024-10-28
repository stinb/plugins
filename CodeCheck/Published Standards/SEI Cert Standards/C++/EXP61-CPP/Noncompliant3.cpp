#include <iostream>

auto make_function()
{
    int x = 3;
    return [&]
    {
      x = 100;
      std::cout << x << '\n';
      return x; // UndCC_Violation
    };
}

int main()
{
    auto f = make_function(); // the use of x in f binds directly to i
}
