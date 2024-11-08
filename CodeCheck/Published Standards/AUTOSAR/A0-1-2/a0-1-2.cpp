// $Id: A0-1-2.cpp 289436 2017-10-04 10:45:23Z michal.szczepankiewicz $
#include <algorithm>
#include <cstdint>
#include <vector>
std::uint8_t Fn1() noexcept
{
  return 0U;
}
void Fn2() noexcept
{
  std::uint8_t x = Fn1(); // UndCC_Valid
  Fn1(); // UndCC_Violation
  static_cast<void>(Fn1()); // UndCC_Valid by exception
}
void Fn3()
{
  std::vector<std::int8_t> v{0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5}; // UndCC_Valid
  std::unique(v.begin(), v.end()); // UndCC_Violation
  v.erase(std::unique(v.begin(), v.end()), v.end());   // UndCC_Violation - however std::unique is compliant
}
