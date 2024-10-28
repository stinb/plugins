#include <iostream>
#include <vector>
void f(const std::vector<int> &c) {
  for (auto i = c.begin(), e = i + 20; i != e; ++i) { // UndCC_Violation(Lin,Win)
    std::cout << *i << std::endl;
  }
}
void f2(const std::vector<int> &c) {
  const int size = 20;
  for (auto i = c.begin(), e = i + size; i != e; ++i) { // UndCC_Violation(Lin,Win)
    i += 2; // UndCC_Violation(Lin,Win)
    std::cout << *i << std::endl;
  }
}
void compliant(const std::vector<int> &c) {
  const std::vector<int>::size_type maxSize = 20;
  for (auto i = c.begin(), e = i + std::min(maxSize, c.size()); i != e; ++i) {
  }
}
