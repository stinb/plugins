#include <cstring>

struct S
{
    int i, j, k;

    // ...

    virtual void f();
};

void f()
{
    S *s = new S;
    // ...
    std::memset(s, 0, sizeof(S));       // UndCC_Violation
    // ...
    s->f(); // undefined behavior
}
