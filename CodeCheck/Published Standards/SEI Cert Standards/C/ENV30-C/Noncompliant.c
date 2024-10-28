#include <stdlib.h>

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
    char *env = getenv("TEST_ENV");
    if (env == NULL)
    {
        /* Handle error */
    }
    trstr(env, '"', '_');       // UndCC_Violation
}
