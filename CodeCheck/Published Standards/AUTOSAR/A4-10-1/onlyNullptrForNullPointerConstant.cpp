//% $Id: A4-10-1.cpp 298086 2017-11-24 11:13:27Z michal.szczepankiewicz $
#include <cstddef>
#include <cstdint>

void F1(std::int32_t);
void F2(std::int32_t *);
void F3()
{
    F1(0);    // UndCC_Valid
    F1(NULL); // UndCC_Violation
    // compilable

    // compilation error
    F2(0);       // UndCC_Violation - 0 used as the null pointer constant
    F2(NULL);    // UndCC_Violation - NULL used as the null pointer constant
    F2(nullptr); // UndCC_Valid
}
void F4(std::int32_t *);
template <class F, class A>
void F5(F f, A a)
{
    F4(a);
}
void F6()
{
    F5(F4, nullptr); // UndCC_Valid
}