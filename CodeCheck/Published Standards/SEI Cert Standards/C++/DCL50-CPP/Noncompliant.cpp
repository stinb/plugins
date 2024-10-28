// SEI CERT C++

#include <cstdarg>

int add(int first, int second, ...) // UndCC_Violation
{
    int r = first + second;
    va_list va;
    va_start(va, second);
    while (int v = va_arg(va, int))
    {
        r += v;
    }
    va_end(va);
    return r;
}
