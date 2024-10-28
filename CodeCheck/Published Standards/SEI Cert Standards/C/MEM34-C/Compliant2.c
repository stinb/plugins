#include <stdlib.h>

enum
{
    BUFSIZE = 256
};

void f(void)
{
    char *buf = (char *)malloc(BUFSIZE * sizeof(char));
    char *p = (char *)realloc(buf, 2 * BUFSIZE);
    if (p == NULL)
    {
        /* Handle error */
    }
}