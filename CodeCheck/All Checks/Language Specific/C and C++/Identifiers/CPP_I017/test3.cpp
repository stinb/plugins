// AUTOSAR C++ 2014

#include <cstdint>

std::int32_t sum = 0;
namespace
{
  std::int32_t sum; // UndCC_Violation - hides sum in outer scope
}
class C1
{
  std::int32_t sum; // UndCC_Valid - does not hide sum in outer scope
};
namespace n1
{
  std::int32_t sum; // UndCC_Valid - does not hide sum in outer scope
  namespace n2
  {
    std::int32_t sum; // UndCC_Valid - does not hide sum in outer scope
  }
}

std::int32_t idx;
void F1(std::int32_t idx) // UndCC_Violation - hides idx in outer scope
{

}

void F2()
{
  std::int32_t max = 5;

  for (std::int32_t idx = 0; idx < max; ++idx) // UndCC_Violation - hides idx in outer scope
  {
    for (std::int32_t idx = 0; idx < max; ++idx) // UndCC_Violation - hides idx in outer scope
    {
    }
  }
}

void F3()
{
  std::int32_t i = 0;
  std::int32_t j = 0;
  auto lambda = [i]() {
    std::int32_t j = 10; // UndCC_Valid - j was not captured, so it does not hide j in outer scope
    return i + j;
  };
}
