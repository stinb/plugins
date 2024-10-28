#include <stdio.h>

void func(const char *filename)
{
    FILE *fileptr = fopen(filename, "rb");
    if (fileptr == NULL)
    {
        /* An error occurred in fopen() */
    }
}