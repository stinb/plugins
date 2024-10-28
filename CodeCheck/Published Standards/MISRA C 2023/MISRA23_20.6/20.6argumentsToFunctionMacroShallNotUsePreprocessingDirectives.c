#define M( A ) printf ( #A )
#include <stdio.h>
int main ( void )
{
	/* UndCC_Violation */ M (
#ifdef SW
		"Message 1"
#else
		"Message 2"
#endif
	);
	return 1;
}
