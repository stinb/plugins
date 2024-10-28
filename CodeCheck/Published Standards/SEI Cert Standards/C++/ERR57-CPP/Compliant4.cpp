#include <memory>

struct A
{ /* ... */
};
struct B
{ /* ... */
};

class C
{
    std::unique_ptr<A> a;
    std::unique_ptr<B> b;

protected:
    void init() noexcept(false);

public:
    C() : a(new A()), b(new B())
    {
        init();
    }
};