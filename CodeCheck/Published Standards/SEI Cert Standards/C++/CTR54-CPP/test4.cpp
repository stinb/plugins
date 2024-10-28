// SEI CERT C++ - CTR54-CPP

#include <iostream>

template <typename Ty>
bool in_range(const Ty *test, const Ty *r, size_t n) {
  auto *cur = reinterpret_cast<const unsigned char *>(r);
  auto *end = reinterpret_cast<const unsigned char *>(r + n);
  auto *testPtr = reinterpret_cast<const unsigned char *>(test);

  for (; cur != end; ++cur) { // UndCC_Valid
    if (cur == testPtr) {     // UndCC_Valid
      return true;
    }
  }
  return false;
}

void f() {
  double foo[10];
  double *x = &foo[0];
  double bar;
  std::cout << std::boolalpha << in_range(&bar, x, 10);
}
