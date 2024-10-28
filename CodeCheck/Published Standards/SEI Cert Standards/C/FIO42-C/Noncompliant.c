#include <stdio.h>
#include <fcntl.h>

#ifndef S_IRUSR
#define S_IRUSR 00400
#endif

int func(const char *filename)
{
    FILE *f = fopen(filename, "r"); // UNDCC_Violation, file not closed
    if (NULL == f)
    {
        return -1;
    }
    /* ... */
    return 0;
}

int func_2(const char *filename)
{
    int fd = open(filename, O_RDONLY, S_IRUSR); // UNDCC_Violation, file not closed
    if (-1 == fd)
    {
        return -1;
    }
    /* ... */
    return 0;
}
