#include <typeinfo>

struct S
{
}; // Non-polymorphic

struct P
{
    virtual void foo() {}
}; // Polymorphic

const std::type_info &foo(S *s)
{
    return typeid(*s); // UndCC_Valid
}
const std::type_info &foo(P *p)
{
    return typeid(*p); // UndCC_Violation
}
const std::type_info &foo(P p)
{
    return typeid(p); // UndCC_Violation
}
const std::type_info &bar(P *p)
{
    return typeid(p->foo()); // UndCC_Valid - type is always 'void'
}
