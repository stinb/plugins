#include <stdint.h>
extern int16_t a;  //okay

extern void violationOne(); // UndCC_Violation
extern int violationTwo; // UndCC_Violation

extern violationThree; // UndCC_Violation

