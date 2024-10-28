// SEI CERT C++

#include <algorithm>
#include <vector>

void f() {
  std::vector<int> v(10);
  std::fill_n(v.begin(), 10, 0x42); // UndCC_Valid
}
