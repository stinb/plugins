// SEI CERT C++

#include "0a.h"
#include <iostream>

void g() {
  std::cout << "g(): " << v << std::endl;
  v = 100;
}

int main() {
  extern void f();
  f(); // Prints v, sets it to 42
  g(); // Prints v, sets it to 100
  f();
  g();
}
