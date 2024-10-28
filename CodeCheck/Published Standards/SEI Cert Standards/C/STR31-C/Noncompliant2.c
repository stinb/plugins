#include <stdio.h>

#define BUFFER_SIZE 1024

void func(void)
{
    char buf[BUFFER_SIZE];
    if (gets(buf) == NULL) // UndCC_Violation
    {
        /* Handle error */
    }
}