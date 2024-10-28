#include <stdio.h>

void func(const char *name)
{
    char filename[128];
    sprintf(filename, "%s.txt", name);
}