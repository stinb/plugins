#include <cstdint>
#include <memory>

struct A
{
    int8_t i;
};

class B
{
};

void f0()
{
    auto p = std::make_shared<A>(); // UndCC_Valid
    int8_t *pi = &(p->i);
    std::shared_ptr<int8_t> q(p, pi); // Does not apply - not taking ownership
}

auto f1()
{
    auto *p1 = new A();
    auto p2 = std::make_unique<A>(); // make_unique may throw
    return std::shared_ptr<A>(p1);   // UndCC_Violation - memory leak if
} // make_unique throws

auto f2(std::unique_ptr<A> p)
{
    auto q = p.get();
    // ...
    return std::unique_ptr<A>(q); // UndCC_Violation - causes double delete
}

void f3(std::shared_ptr<A> a, std::shared_ptr<B> b);

void f4()
{
    f3(std::shared_ptr<A>(new A()),  // UndCC_Violation
       std::shared_ptr<B>(new B())); // UndCC_Violation - but well defined
} // in C++17

void f5()
{
    f3(std::make_shared<A>(),
       std::make_shared<B>()); // UndCC_Valid
}
