// SEI CERT C++

#include <algorithm>
#include <iostream>
#include <vector>

void f(std::vector<int> &c) {
  c.erase(std::remove(c.begin(), c.end(), 42), c.end()); // UndCC_Valid
  for (auto v : c) {
    std::cout << "Container element: " << v << std::endl;
  }
}
