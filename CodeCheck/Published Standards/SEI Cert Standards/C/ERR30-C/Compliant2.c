#include <errno.h>
#include <stdio.h>

void func(FILE *fp)
{
    if (ftell(fp) == -1)
    {
        perror("ftell");
    }
}