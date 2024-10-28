#include <stdarg.h>
#include <stddef.h>

void func(size_t num_vargs, ...)
{
    va_list ap;
    va_start(ap, num_vargs);
    if (num_vargs > 0)
    {
        unsigned char c = va_arg(ap, unsigned char); // UndCC_Violation
        // ...
    }
    va_end(ap);
}

void f(void)
{
    unsigned char c = 0x12;
    int x = 1;
    size_t y = 0;
    func(y, c);
}

void func1(const char *cp, ...) // UndCC_Violation, initial parameter should be number of variadic arguments passed
{
    va_list ap;
    va_start(ap, cp);
    int val = va_arg(ap, int);
    // ...
    va_end(ap);
}

void f1(void)
{
    func1("The only argument"); 
}
