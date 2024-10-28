// File2.cpp
#include "File2.h"
#include "File1.h"
#include <iostream>

class Hah
{
protected:
    short s;

public:
    Hah() : s(12) {}
};

class D : public Hah, public B
{
    float f;

public:
    D() : Hah(), B(), f(1.2f) {}
    void do_something() { std::cout << "f: " << f << ", d: " << d << ", s: " << s << std::endl; }
};

void g(D *d)
{
    d->do_something();
}

B *get_d()
{
    return new D;
}