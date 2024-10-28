// SEI CERT C++

#include <random>
#include <iostream>

void f() {
  std::random_device dev;
  std::mt19937 engine(dev()); // UndCC_Valid

  for (int i = 0; i < 10; ++i) {
    std::cout << engine() << ", ";
  }
}
