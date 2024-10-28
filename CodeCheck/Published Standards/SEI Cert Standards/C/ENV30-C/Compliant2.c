
#include <stdlib.h>
#include <string.h>

void trstr(char *c_str, char orig, char rep)
{
    while (*c_str != '\0')
    {
        if (*c_str == orig)
        {
            *c_str = rep;
        }
        ++c_str;
    }
}

void func(void)
{
    const char *env;
    char *copy_of_env;

    env = getenv("TEST_ENV");
    if (env == NULL)
    {
        /* Handle error */
    }

    copy_of_env = strdup(env);
    if (copy_of_env == NULL)
    {
        /* Handle error */
    }

    trstr(copy_of_env, '"', '_');

    if (setenv("TEST_ENV", copy_of_env, 1) != 0)
    {
        /* Handle error */
    }
    /* ... */
    free(copy_of_env);
}