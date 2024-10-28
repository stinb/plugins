#include <ctype.h>
#include <string.h>

size_t count_preceding_whitespace(const char *s)
{
    const char *t = s;
    size_t length = strlen(s) + 1;
    while (isspace(*t) && (t - s < length)) // UndCC_Violation
    {
        ++t;
    }
    isascii(*t); // UndCC_Violation
    return t - s;
}