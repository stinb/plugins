// SEI CERT C++

#include <algorithm>
#include <vector>

void f(const std::vector<int> &src) {
  // Initialize dest with src.size() default-inserted elements
  std::vector<int> dest(src.size());
  std::copy(src.begin(), src.end(), dest.begin()); // UndCC_Valid
  // ...
}
