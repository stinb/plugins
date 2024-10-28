#include <stdarg.h>
#include <stdio.h>

int contains_zero(size_t count, va_list *ap)
{
    va_list ap1;
    va_copy(ap1, *ap);
    for (size_t i = 1; i < count; ++i)
    {
        if (va_arg(ap1, double) == 0.0)
        {
            return 1;
        }
    }
    va_end(ap1);
    return 0;
}

int print_reciprocals(size_t count, ...)
{
    int status;
    va_list ap;
    va_start(ap, count);

    if (contains_zero(count, &ap))
    {
        printf("0 in arguments!\n");
        status = 1;
    }
    else
    {
        for (size_t i = 0; i < count; i++)
        {
            printf("%f ", 1.0 / va_arg(ap, double));
        }
        printf("\n");
        status = 0;
    }

    va_end(ap);
    return status;
}