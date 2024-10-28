#include <stdint.h>
#define QUALIFIER volatile

void f(QUALIFIER int32_t p)
{
    while (p != 0)
    {
        ; /* Wait... */
    }
}

#undef QUALIFIER /* UndCC_Violation */