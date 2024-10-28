// SEI CERT C++ - CTR54-CPP

#include <cstddef>
#include <iostream>

template <typename Ty>
bool in_range(const Ty *test, const Ty *r, size_t n) {
  return 0 < (test - r) && (test - r) < (std::ptrdiff_t) n; // UndCC_Violation
}

void f() {
  double foo[10];
  double *x = &foo[0];
  double bar;
  std::cout << std::boolalpha << in_range(&bar, x, 10);
}
