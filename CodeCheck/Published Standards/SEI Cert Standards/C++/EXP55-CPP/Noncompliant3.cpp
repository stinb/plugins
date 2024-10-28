#include <iostream>

struct S
{
    int i;

    S(int i) : i(i) {}
};

void g(S &s)
{
    std::cout << s.i << std::endl;
}

void f()
{
    volatile S s(12);
    g(const_cast<S &>(s)); // UndCC_Violation
}