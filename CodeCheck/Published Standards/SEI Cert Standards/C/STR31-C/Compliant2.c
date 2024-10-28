#include <stdio.h>
#include <string.h>

enum
{
    BUFFERSIZE = 32
};

void func(void)
{
    char buf[BUFFERSIZE];
    int ch;

    if (fgets(buf, sizeof(buf), stdin))
    {
        /* fgets() succeeded; scan for newline character */
        char *p = strchr(buf, '\n');
        if (p)
        {
            *p = '\0';
        }
        else
        {
            /* Newline not found; flush stdin to end of line */
            while ((ch = getchar()) != '\n' && ch != EOF)
                ;
            if (ch == EOF && !feof(stdin) && !ferror(stdin))
            {
                /* Character resembles EOF; handle error */
            }
        }
    }
    else
    {
        /* fgets() failed; handle error */
    }
}