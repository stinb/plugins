#include <string.h>

void func(const char *src)
{
    /* UndCC_Validate the source string; calculate size */
    char *dest;
/* malloc() destination string */
#ifdef PLATFORM1
    memcpy(dest, src, 12);
#else
    memcpy(dest, src, 24);
#endif
    /* ... */
}