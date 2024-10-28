#include <cstddef>

struct D
{
    virtual void f() {}
    struct InnerStandardLayout
    {
        int i;
    } inner;
};

void f()
{
    size_t off = offsetof(D::InnerStandardLayout, i);
    // ...
}