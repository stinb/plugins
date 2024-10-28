#include <cstdint>

int16_t usedtype() {
  typedef int16_t used_local_t; // UndCC_Valid
  used_local_t result = 67;
  return result;
}

int16_t unusedtype() {
  typedef int16_t local_Type; // UndCC_Violation
  return 67;
}
