#include <stddef.h>

void copy(size_t n, char src[n], char dest[n])
{
    size_t i;

    for (i = 0; src[i] && (i < n - 1); ++i)
    {
        dest[i] = src[i];
    }
    dest[i] = '\0';
}