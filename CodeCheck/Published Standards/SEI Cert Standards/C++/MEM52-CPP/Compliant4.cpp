#include <memory>

struct A
{ /* ... */
};
struct B
{ /* ... */
};

void g(std::unique_ptr<A> a, std::unique_ptr<B> b);
void f()
{
    g(std::make_unique<A>(), std::make_unique<B>());
}