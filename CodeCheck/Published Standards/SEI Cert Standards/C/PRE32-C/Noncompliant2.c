
#include <string.h>

void func2(const char *src)
{
    /* UndCC_Validate the source string; calculate size */
    char *dest;
    /* malloc() destination string */
    memcpy(dest, src, // UNDCC_Violation, using preprocessor in argument
#ifndef PLATFORM1
           24
#else
           12
#endif
    );
    /* ... */
}
