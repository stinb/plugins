// SEI CERT C++

#include "1a.h"
#include <iostream>

int v; // Definition of global variable v

void f() {
  std::cout << "f(): " << v << std::endl;
  v = 42;
  // ...
}
