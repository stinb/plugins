#include <string.h>

void f ( void )
{
	int p[3];
	int q[3];
	int n;
	/* memcpy has restrict-qualified parameters */
	memcpy ( p, q, n );
}

void user_copy ( void * restrict p_1, void * restrict q_1, int n_1 )  // UndCC_Violation
{
}

int i;
i = 265;
