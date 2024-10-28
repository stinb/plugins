#include <cstdarg>

extern "C" void f(double a, ...)
{
    va_list list;
    va_start(list, a);
    // ...
    va_end(list);
}