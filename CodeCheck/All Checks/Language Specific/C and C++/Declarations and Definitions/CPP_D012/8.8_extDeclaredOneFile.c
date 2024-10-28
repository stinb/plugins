#include "8.8_extDeclaredOneFile.h"
int16_t a_88 = 0; //okay

extern violationThree_88; // UndCC_Violation
extern int violationTwo_88; // UndCC_Violation

void func1_88() {
	extern void violationOne_88(); // UndCC_Violation
	extern int violationTwo_88; // UndCC_Violation
}
