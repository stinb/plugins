#include <stdlib.h>
#include <stdio.h>

int func(const char *var)
{
    const char *env_format = "TEST=%s";
    const size_t len = strlen(var) + strlen(env_format);
    char *env = (char *)malloc(len);
    if (env == NULL)
    {
        return -1;
    }
    int retval = snprintf(env, len, env_format, var);
    if (retval < 0 || (size_t)retval >= len)
    {
        /* Handle error */
    }
    if (putenv(env) != 0)
    {
        free(env);
        return -1;
    }
    return 0;
}