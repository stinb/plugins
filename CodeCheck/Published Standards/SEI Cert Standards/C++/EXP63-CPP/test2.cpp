// SEI CERT C++

#include <algorithm>
#include <iostream>
#include <vector>

void f(std::vector<int> &c) {
  std::remove(c.begin(), c.end(), 42); // UndCC_Violation
  for (auto v : c) {
    std::cout << "Container element: " << v << std::endl;
  }
}
