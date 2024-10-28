// Comment preceding preprocessor directives!

#include <cstdint> // UndCC_Valid
int32_t a;

#include <f1.h> // UndCC_Violation
int32_t i;

#include <f2.h> // UndCC_Violation

#ifndef SHA1DC_NO_STANDARD_INCLUDES
#include <stdint.h>     // UndCC_Violation
#endif
