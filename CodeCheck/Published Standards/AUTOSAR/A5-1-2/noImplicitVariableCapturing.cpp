// $Id: A5-1-2.cpp 289436 2017-10-04 10:45:23Z michal.szczepankiewicz $
#include <algorithm>
#include <cstdint>
#include <vector>
void Fn1(const std::vector<std::int32_t>& v)
{
  std::uint64_t sum = 0;  
  std::for_each(v.begin(), v.end(), [&](std::int32_t lhs) { // UndCC_Violation
    sum += lhs;
    }); 
  
  sum = 0;
  std::for_each(v.begin(), v.end(), [&sum](std::int32_t lhs) {
    sum += lhs;
  }); // UndCC_Valid
}

void Fn2()
{
  constexpr std::uint8_t n = 10;
  static std::int32_t j = 0;
  [n]() {
    std::int32_t array[n]; // UndCC_Valid
    j += 1; // UndCC_Valid by exception
  };
}
