#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

typedef double float64_t;

void someFunction(void) {}
void handleError() {}

void f1(void)
{
    errno = 0;
    float64_t f64;

    f64 = strtod("A.12", NULL); // UndCC_Violation
    someFunction();             /* Non-compliant - function call */
    if (0 != errno)
    {
    }

    errno = 0;
    f64 = strtod("A.12", NULL);
    if (0 != errno) /* UndCC_Valid */
    {
    }
}

void f2(FILE *f, fpos_t *pos)
{
    errno = 0;
    if (fsetpos(f, pos) == 0) /* UndCC_Valid by exception -
    no need to test errno as no out-of-band error reported. */
    {
        // ...
    }
    else
    {
        /* Something went wrong - errno holds an implementation-defined positive value.
         */
        handleError(errno);
    }
}