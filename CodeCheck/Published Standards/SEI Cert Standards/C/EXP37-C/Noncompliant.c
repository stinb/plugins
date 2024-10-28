#ifndef _WIN32

#include <tgmath.h>

void func(void)
{
    double complex c = 2.0 + 4.0 * I;
    double complex result = log2((double)c); // UndCC_Violation
}

#endif // not _WIN32
