// SEI CERT C++

#include <ctime>
#include <random>
#include <iostream>

void f() {
  std::time_t t;
  std::mt19937 engine(std::time(&t)); // UndCC_Violation

  for (int i = 0; i < 10; ++i) {
    std::cout << engine() << ", ";
  }
}
