
#include <string.h>

void func(const char *src)
{
    /* UndCC_Validate the source string; calculate size */
    char *dest;
    /* malloc() destination string */
    memcpy(dest, src, // UNDCC_Violation, using preprocessor in argument
#ifdef PLATFORM1
           12
#else
           24
#endif
    );
    /* ... */
}