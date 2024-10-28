#include "test1.h"

int sharedNumber = 10; // UndCC_Violation

static int privateNumber = 20;

/* this is an extremely common mistake that can get really nasty -- a global
 * variable that is exported by the linker, but never "extern" referenced, so no
 * other files can take advantage of the export, but it's there, and collides
 * with any other exported global in any other file, which can cause obvious and
 * undesireable consequences */
int commonMistake = 30; // UndCC_Violation

int getShared() { // UndCC_Violation
	return sharedNumber;
}
