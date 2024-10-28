// SEI CERT C++

#include <algorithm>
#include <vector>

void f(const std::vector<int> &src) {
  std::vector<int> dest;
  std::copy(src.begin(), src.end(), dest.begin()); // UndCC_Violation(Win,Lin)
  // ...
}
