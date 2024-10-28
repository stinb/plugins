// SEI CERT C++

#include "compliant_library.h"

void f() {
  if (int err = func()) { // UndCC_Valid
    // Handle error
  }
}
