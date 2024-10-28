#include <f1.h> // UndCC_Valid
int32_t i;
#include <f2.h> // UndCC_Violation
#if 0
#include <f3.h> // UndCC_Valid - would normally be a violation but inactive code is ignored
#endif
