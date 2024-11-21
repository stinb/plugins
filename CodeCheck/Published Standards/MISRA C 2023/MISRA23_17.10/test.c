#include <stdint.h>

_Noreturn int32_t f(void); /* UndCC_Violation */
_Noreturn void g(void);    /* UndCC_Valid */
