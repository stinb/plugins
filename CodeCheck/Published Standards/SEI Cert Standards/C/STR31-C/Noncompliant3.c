#include <stdio.h>

enum
{
    BUFFERSIZE = 32
};

void func(void)
{
    char buf[BUFFERSIZE];
    char *p;
    int ch;
    p = buf;
    while ((ch = getchar()) != '\n' && ch != EOF)
    {
        *p++ = (char)ch;
    }
    *p++ = 0;
    if (ch == EOF)
    {
        /* Handle EOF or error */
    }
}