// SEI CERT C++

#include <cstddef>
#include <vector>

void f(std::vector<int> &v, size_t s) {
  v.resize(s); // UndCC_Valid
}
