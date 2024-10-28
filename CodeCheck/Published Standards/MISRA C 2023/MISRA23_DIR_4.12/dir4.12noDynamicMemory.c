
#include <stdlib.h>
void f ( void )
{
	char *p = ( char * ) malloc ( 10 ); // UndCC_Violation
	char *q;
	free ( p ); // UndCC_Violation
	q = p; /* Undefined behaviour - value of p is indeterminate */
	p = ( char * ) malloc ( 20 ); // UndCC_Violation
	free ( p ); // UndCC_Violation
	p = NULL; /* Assigning NULL to freed pointer makes it determinate */
}
