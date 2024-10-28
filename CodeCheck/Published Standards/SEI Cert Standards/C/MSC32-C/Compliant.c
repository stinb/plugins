
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void func(void)
{
    struct timespec ts;
    if (timespec_get(&ts, TIME_UTC) == 0)
    {
        /* Handle error */
    }
    else
    {
        srandom(ts.tv_nsec ^ ts.tv_sec);
        for (unsigned int i = 0; i < 10; ++i)
        {
            /* Generates different sequences at different runs */
            printf("%ld, ", random());  /* UndCC_Valid */
        }
    }
}
