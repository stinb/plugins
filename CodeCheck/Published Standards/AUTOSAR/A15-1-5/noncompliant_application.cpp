// SEI CERT C++

#include "noncompliant_library.h"

void f() {
  try {
    func(); // UndCC_Violation(noncompliant)
  } catch(int &e) {
    // Handle error
  }
}
