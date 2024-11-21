// SEI CERT C++

#include "noncompliant_library.h"

void f() {
  try {
    func(); // UndCC_Violation
  } catch(int &e) {
    // Handle error
  }
}
