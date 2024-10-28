#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
    typedef size_t ssize_t;
#endif

void func(void)
{
    int ch;
    size_t buffer_size = 32;
    char *buffer = malloc(buffer_size);

    if (!buffer)
    {
        /* Handle error */
        return;
    }

    ssize_t size = getline(&buffer, &buffer_size, stdin);

    if (size == -1)
    {
        /* Handle error */
    }
    else
    {
        char *p = strchr(buffer, '\n');
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
    free(buffer);
}
