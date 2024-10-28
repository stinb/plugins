#include <stdio.h>

void func(const char *name)
{
    char filename[128];
    sprintf(filename, "%.123s.txt", name);
}

void func_2(const char *name)
{
    char filename[128];
    sprintf(filename, "%.*s.txt", sizeof(filename) - 5, name);
}

void func_3(const char *name)
{
    char filename[128];
    int result = snprintf(filename, sizeof(filename), "%s.txt", name);
    if (result != strlen(filename))
    {
        /* truncation occurred */
    }
}