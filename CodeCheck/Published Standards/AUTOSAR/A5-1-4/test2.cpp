// Custom

#include <cstdint>
#include <functional>

std::function<int()> F()
{

  int a = 1000; // UndCC_Violation
  return ([&a]() -> int {

    a = 10;
    return a;

  });
}
