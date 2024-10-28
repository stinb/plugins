#include <string.h>
#include <stdlib.h>

void f(const char *input_str)
{
    size_t size = strlen(input_str) + 1;
    char *c_str = (char *)malloc(size);
    memcpy(c_str, input_str, size); // UndCC_Violation
    /* ... */
    free(c_str);
    c_str = NULL;
    /* ... */
}
