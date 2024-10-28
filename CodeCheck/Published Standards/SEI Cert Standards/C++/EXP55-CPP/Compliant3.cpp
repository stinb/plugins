#include <iostream>

struct S
{
    int i;

    S(int i) : i(i) {}
};

void g(volatile S &s)
{
    std::cout << s.i << std::endl;
}

void f()
{
    volatile S s(12);
    g(s);
}