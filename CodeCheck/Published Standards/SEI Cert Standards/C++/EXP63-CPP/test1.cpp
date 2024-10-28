// SEI CERT C++

#include <iostream>
#include <string>

void g(std::string v) {
  std::cout << v << std::endl;
}

void f() {
  for (unsigned i = 0; i < 10; ++i) {
    std::string s(1, static_cast<char>('0' + i));
    g(std::move(s)); // UndCC_Valid
  }
}
