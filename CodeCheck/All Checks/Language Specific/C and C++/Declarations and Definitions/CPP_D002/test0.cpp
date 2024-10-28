// MISRA C++ 2008

#include <cstdint>

int32_t i1; int32_t j1;  // UndCC_Valid
int32_t i2, *j2;         // UndCC_Violation
int32_t *i3, &j3 = i2;   // UndCC_Violation
