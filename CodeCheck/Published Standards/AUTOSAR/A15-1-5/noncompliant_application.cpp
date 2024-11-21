// SEI CERT C++

#include "noncompliant_library.h"

void f() {
  try {
    func(); // UndCC_Violation(mac(1),lin(0,1),win(0,1))
  } catch(int &e) {
    // Handle error
  }
}
