/*#include <stdio.h> // UndCC_Violation
#include "regexp.h"
*/

// #define END 0 okay // UndCC_Violation



/* // UndCC_Violation
#ifndef CHARBITS
#define	UCHARAT(p)	((int)*(unsigned char *)(p))
#endif
*/

// Regular comment - UndCC_Valid
// Versions 1.0 - 2 UndCC_Valid

// ++i; // UndCC_Violation

char *exp;

int main()
{
	extern char *malloc();

//	if (exp == NULL)           // UndCC_Violation
//		FAIL("NULL argument"); // UndCC_Valid - The line above is flagged

 int i;
	/* // UndCC_Violation
	regparse = exp;
	regnpar = 1;
*/

	/*	if (flags&SPSTART) { // UndCC_Violation
		}*/

  /* while (something){}*/   // UndCC_Violation


	return i;
}
