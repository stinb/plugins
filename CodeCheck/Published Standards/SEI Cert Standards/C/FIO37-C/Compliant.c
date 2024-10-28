#include <stdio.h>
#include <string.h>

enum
{
    BUFFER_SIZE = 1024
};

void func(void)
{
    char buf[BUFFER_SIZE];
    char *p;

    if (fgets(buf, sizeof(buf), stdin))
    {
        p = strchr(buf, '\n');
        if (p)
        {
            *p = '\0';
        }
    }
    else
    {
        /* Handle error */
    }
}

void func_2(void)
{
    char buf[BUFFER_SIZE];
    char *p;

    while (fgets(buf, sizeof(buf), stdin))
    {
        if (!strcmp(buf, "\n"))
            break;
        buf[strlen(buf) - 1] = '\0';
    }
}
