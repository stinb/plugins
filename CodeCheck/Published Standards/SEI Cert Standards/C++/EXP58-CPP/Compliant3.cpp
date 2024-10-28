#include <cstdarg>
#include <iostream>

extern "C" void f(const char *s, ...)
{
    va_list list;
    va_start(list, s);
    std::cout << (s ? s : "") << ", " << va_arg(list, int);
    va_end(list);
}