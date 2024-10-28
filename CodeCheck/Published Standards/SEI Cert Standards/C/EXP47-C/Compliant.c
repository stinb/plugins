#include <stdarg.h>
#include <stddef.h>

void func(size_t num_vargs, ...)
{
    va_list ap;
    va_start(ap, num_vargs);
    if (num_vargs > 0)
    {
        unsigned char c = (unsigned char)va_arg(ap, int);
        // ...
    }
    va_end(ap);
}

void f(void)
{
    unsigned char c = 0x12;
    func(1, c);
}

void func1(size_t num_vargs, const char *cp, ...)
{
    va_list ap;
    va_start(ap, cp);
    if (num_vargs > 0)
    {
        int val = va_arg(ap, int);
        // ...
    }
    va_end(ap);
}

void f1(void)
{
    func(0, "The only argument");
}