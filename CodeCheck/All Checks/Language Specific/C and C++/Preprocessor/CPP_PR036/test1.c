// MISRA C 2012

#include <stdint.h>

#define AAA 2

int32_t foo ( void )
{
	int32_t x = 0;

	#ifndef AAA
		x = 1;
	#else1 // UndCC_Violation
		x = AAA;
	#endif
		return x;
}
