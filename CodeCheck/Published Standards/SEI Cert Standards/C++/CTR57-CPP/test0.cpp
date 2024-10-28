// SEI CERT C++

#include <functional>
#include <iostream>
#include <set>

void f() {
  std::set<int, std::less_equal<int>> s{5, 10, 20}; // UndCC_Violation(Win,Lin)
  for (auto r = s.equal_range(10); r.first != r.second; ++r.first) {
    std::cout << *r.first << std::endl;
  }
}
