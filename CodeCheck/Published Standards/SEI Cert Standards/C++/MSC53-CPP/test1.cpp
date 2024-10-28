// SEI CERT C++

#include <cstdlib>

[[noreturn]] void f(int i) {
  if (i > 0)
    throw "Received positive input";
  std::exit(0);
} // UndCC_Valid
