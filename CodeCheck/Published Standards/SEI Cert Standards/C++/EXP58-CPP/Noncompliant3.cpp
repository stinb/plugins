#include <cstdarg>
#include <iostream>
#include <string>

extern "C" void f(std::string s, ...)
{
    va_list list;
    va_start(list, s); // UndCC_Violation
    std::cout << s << ", " << va_arg(list, int);
    va_end(list);
}