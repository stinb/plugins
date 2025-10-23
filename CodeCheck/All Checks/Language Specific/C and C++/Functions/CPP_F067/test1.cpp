// SEI CERT C++

#include <cstdlib>

[[noreturn]] void f(int i) {
  if (i > 0)
    throw "Received positive input";
  else if (i < 0)
    std::exit(0);
} // UndCC_Violation
