#include <stdlib.h>
#include <stdio.h>

int func(const char *var)
{
    char env[1024];
    int retval = snprintf(env, sizeof(env), "TEST=%s", var);
    if (retval < 0 || (size_t)retval >= sizeof(env))
    {
        /* Handle error */
    }

    return putenv(env); // UndCC_Violation
}