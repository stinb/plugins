#include <cstdint>

const int16_t x = 19;      // UndCC_Valid
const int16_t y = 21;      // UndCC_Violation
void usedonlyonce ( void )
{
	int16_t once_1 = 42;   // UndCC_Violation
	int16_t once_2;        // UndCC_Violation
	once_2 = x;
}

const int16_t *p = &x;     // UndCC_Valid
const int16_t *q = p;      // UndCC_Violation
