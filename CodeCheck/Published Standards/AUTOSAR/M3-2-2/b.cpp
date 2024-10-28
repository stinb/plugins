#include <cstdint>


struct S1                     // UndCC_Violation
{
	int64_t i;
};

struct S2                     // UndCC_Violation
{
	int32_t i;
	int32_t j;
};


typedef uint16_t i16;         // UndCC_Violation

typedef uint32_t i32;         // UndCC_Violation


inline int32_t f(int32_t x) { // UndCC_Violation
	return x / 2;
}

int32_t g(int32_t x) {        // UndCC_Valid - not inline
	return x - 2;
}

inline int32_t h(int32_t x) { // UndCC_Valid - token sequence same
	return x + 4;
}
