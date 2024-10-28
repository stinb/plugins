#include "something.h"
#if !defined HDR // UndCC_Valid
#define HDR // UndCC_Valid
#define X(Y) (Y) // UndCC_Violation
#endif
