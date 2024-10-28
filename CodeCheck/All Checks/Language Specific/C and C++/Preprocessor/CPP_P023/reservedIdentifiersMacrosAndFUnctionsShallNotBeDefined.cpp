#define __TIME__ 11111111 // UndCC_Violation
#undef __LINE__ // UndCC_Violation
#define _hello_ // UndCC_Valid
#define _Bad_ // UndCC_Violation
#define assert // UndCC_Violation
int foo = __TIME__;
