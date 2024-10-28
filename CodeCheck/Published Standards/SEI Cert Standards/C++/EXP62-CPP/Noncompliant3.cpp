// Custom

#include <cstring>

class C
{
    unsigned char buffType;
    int size;
};

void f(const C &c1, const C &c2)
{
    if (!std::memcmp(&c1, &c2, sizeof(C)))      // UndCC_Violation
    {
        // ...
    }
}

class D
{
    int i, j, k;

    // ...
public:
    virtual void f();
};

void f()
{
    D *d = new D;
    // ...
    std::memset(d, 0, sizeof(D));       // UndCC_Violation
    // ...
    d->f(); // undefined behavior
}
