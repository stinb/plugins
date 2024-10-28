#include <cstdint>

int32_t x, y, z;
bool q, w, r;

int32_t a = x ? y : z;       // UndCC_Violation
int32_t b = q ? x : z;       // UndCC_Valid
int32_t c = (z < 5) ? x : y; // UndCC_Valid