// SEI CERT C++

#include <random>
#include <iostream>

void f() {
  std::mt19937 engine; // UndCC_Violation

  for (int i = 0; i < 10; ++i) {
    std::cout << engine() << ", ";
  }
}
