#include <stdlib.h>
#include <errno.h>

typedef double float64_t;

void f(void)
{
    float64_t f64;
    f64 = strtod("A.12", NULL); /* UndCC_Violation */
    if (errno == 0)
    {
        f64 = strtod("A.12", NULL); /* UndCC_Valid, exception */
        /* f64 may not have a valid value in here. */
    }
    errno = 0;
    f64 = strtod("A.12", NULL);
    if (0 == errno) /* UndCC_Valid */
    {
        /* f64 will have a valid value in here. */
    }
}