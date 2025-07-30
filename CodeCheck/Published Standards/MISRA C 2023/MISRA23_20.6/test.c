#define M( A ) printf ( #A )

#include <stdio.h>

void main ( void )
{
M (
#ifdef SW /* UndCC_Violation */
"Message 1"
#else /* UndCC_Violation */
"Message 2"
#endif /* UndCC_Violation */
);
}