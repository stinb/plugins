#define __TIME__ 11111111 // UndCC_Violation
#undef __LINE__ // UndCC_Violation
#define _hello_ // UndCC_Valid
#ifdef NOT_DEFINED
#define _Good_ 2// UndCC_Valid - should not throw in inactive code despite being a violation
#endif
#define _Bad_ // UndCC_Violation
#define assert // UndCC_Violation
int foo = __TIME__;
