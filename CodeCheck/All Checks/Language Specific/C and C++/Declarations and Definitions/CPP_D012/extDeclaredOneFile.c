#include "extDeclaredOneFile.h"
int16_t a = 0; //okay

extern violationThree; // UndCC_Violation
extern int violationTwo; // UndCC_Violation

void func1() {
	extern void violationOne(); // UndCC_Violation
	extern int violationTwo; // UndCC_Violation
}
