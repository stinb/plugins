#include "test1.h"
#include "test2.h"

int getShared(); /* multiple prototypes are very much OK for 8.9 */

int sharedNumber = 20; // UndCC_Violation
\
static int privateNumber = 30;

/* see comment in test1.c */
double commonMistake = 3.14159; // UndCC_Violation

int getShared() { // UndCC_Violation
	return sharedNumber;
}
