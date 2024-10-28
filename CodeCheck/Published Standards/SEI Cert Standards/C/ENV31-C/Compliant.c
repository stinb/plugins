#include <stdio.h>
#include <stdlib.h>

extern char **environ;

int main(void)
{
    if (setenv("MY_NEW_VAR", "new_value", 1) != 0)
    {
        /* Handle error */
    }
    if (environ != NULL)
    {
        for (size_t i = 0; environ[i] != NULL; ++i)
        {
            puts(environ[i]);
        }
    }
    return 0;
}