#include <stdlib.h>
#include <stdint.h>
#include <time.h>

void function(size_t len)
{
    long *p;
    if (len == 0 || len > SIZE_MAX / sizeof(long))
    {
        /* Handle overflow */
    }
    p = (long *)malloc(len * sizeof(int)); // UNDCC_Violation, allocated memory is int
    if (p == NULL)
    {
        /* Handle error */
    }
    free(p);
}
