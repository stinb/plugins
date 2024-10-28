#include <memory>

struct S
{
};

void f()
{
    std::shared_ptr<S> s{new S[10], [](const S *ptr)
                         { delete[] ptr; }};
}