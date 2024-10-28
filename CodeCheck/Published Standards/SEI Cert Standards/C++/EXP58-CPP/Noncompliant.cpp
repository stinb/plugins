#include <cstdarg>

extern "C" void f(float a, ...)
{
    va_list list;
    va_start(list, a); // UndCC_Violation
    // ...
    va_end(list);
}