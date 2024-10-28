#include <complex.h>
#include <math.h>

static inline int iabs(int x)
{
    return (((x) < 0) ? -(x) : (x));
}

void func(int n)
{
    /* Validate that n is within the desired range */

    int m = iabs(++n);

    /* ... */
}