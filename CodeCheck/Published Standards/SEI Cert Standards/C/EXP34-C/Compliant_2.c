#include <string.h>
#include <stdlib.h>

void f(const char *input_str)
{
    size_t size;
    char *c_str;

    if (NULL == input_str)
    {
        /* Handle error */
    }

    size = strlen(input_str) + 1;
    c_str = (char *)malloc(size);
    if (NULL == c_str)
    {
        /* Handle error */
    }
    memcpy(c_str, input_str, size);
    /* ... */
    free(c_str);
    c_str = NULL;
    /* ... */
}