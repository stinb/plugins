// Custom

#include <cstdint>

char *ptr = nullptr;
int64_t a = (int64_t) ptr;                  // UndCC_Violation
int64_t b = reinterpret_cast<int64_t>(ptr); // UndCC_Violation
