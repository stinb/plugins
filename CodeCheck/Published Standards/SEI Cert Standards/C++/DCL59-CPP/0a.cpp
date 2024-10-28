// SEI CERT C++

#include "0a.h"
#include <iostream>

void f() {
  std::cout << "f(): " << v << std::endl;
  v = 42;
  // ...
}
