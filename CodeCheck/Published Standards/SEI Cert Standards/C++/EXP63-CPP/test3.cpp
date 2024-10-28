// SEI CERT C++

#include <algorithm>
#include <iostream>
#include <vector>

void f(std::vector<int> &c) {
  auto e = std::remove(c.begin(), c.end(), 42); // UndCC_Valid
  for (auto i = c.begin(); i != c.end(); i++) {
    if (i < e) {
      std::cout << *i << std::endl;
    }
  }
}
