// SEI CERT C++ - CTR54-CPP

#include <functional>
#include <iostream>

template <typename Ty>
bool in_range(const Ty *test, const Ty *r, size_t n) {
  std::less<const Ty *> less;
  return !less(test, r) && less(test, r + n); // UndCC_Violation(Lin,Win)
}

void f() {
  double foo[10];
  double *x = &foo[0];
  double bar;
  std::cout << std::boolalpha << in_range(&bar, x, 10);
}
