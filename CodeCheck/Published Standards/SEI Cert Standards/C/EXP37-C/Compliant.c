#ifndef _WIN32

#include <tgmath.h>

void func(void)
{
    double complex c = 2.0 + 4.0 * I;
    double complex result = log(c) / log(2);
}

#endif // not _WIN32
