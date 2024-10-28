#include <cstddef>

struct D
{
    virtual void f() {}
    int i;
};

void f()
{
    size_t off = offsetof(D, i); // UndCC_Violation
    // ...
}