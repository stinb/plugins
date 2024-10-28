// Custom

#include <algorithm>
#include <cstdint>
#include <tuple>
#include <vector>

void test_1a()
{
  int32_t a;           // UndCC_Valid
  int32_t b, *c;       // UndCC_Violation
  int32_t *d, &e = b;  // UndCC_Violation
  int32_t f, g;        // UndCC_Violation
  f, g = 0;            // UndCC_Valid
}

std::tuple<int, int, int> firstThree(const std::vector<int> &a)
{
  if (a.size() > 2)
    return std::tuple<int, int, int>(a[0], a[1], a[2]);
  else
    return std::tuple<int, int, int>();
}

void test_1b(const std::vector<int> &a)
{
  const auto [a_min, a_max] = std::minmax_element(a.begin(), a.end());
  const auto [a_0, b_1, c_2] = firstThree(a);
}
