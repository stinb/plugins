#include <errno.h>
#include <limits.h>
#include <stdlib.h>

void func(const char *c_str)
{
    unsigned long number;
    char *endptr;

    number = strtoul(c_str, &endptr, 0); // UndCC_Violation - fails to set errno to 0 before invoking strtoul()
    if (endptr == c_str || (number == ULONG_MAX && errno == ERANGE))
    {
        /* Handle error */
    }
    else
    {
        /* Computation succeeded */
    }
}