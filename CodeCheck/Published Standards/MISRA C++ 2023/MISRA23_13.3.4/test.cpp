#include <cstdint>

class A
{
public:
    void f1();
    void f2();
    virtual void f3();
};

void foo()
{
    if (&A::f1 != &A::f2) // UndCC_Valid
    {
    }
    if (&A::f1 != nullptr) // UndCC_Valid
    {
    }
    if (&A::f3 == &A::f2) // UndCC_Violation - f3 virtual
    {
    }
    if (&A::f3 == nullptr) // UndCC_Valid
    {
    }
}

void foo2()
{
    auto pmf = &A::f3;
    if (pmf == &A::f2) // UndCC_Violation
    {
    }
}

void bar(void (A::*ptr)())
{
    if (ptr == &A::f2) // UndCC_Violation - ptr potentially points to A::f3, which is virtual
    {
    }
}

// Note : the example above would be UndCC_Valid if A had no virtual members.
class B
{
public:
    void f1();
    void f2();
    virtual void f3(int32_t i);
};
void bar(void (B::*ptr)())
{
    if (ptr == &B::f2) // UndCC_Valid - there are no virtual functions in B with the appropriate signature
    {
    }
}
class D : public A // Inherits virtual functions from A
{
public:
    void f4();
};
void car(void (D::*ptr)())
{
    if (ptr == &D::f4) // UndCC_Violation - ptr potentially points to A::f3, which is virtual
    {
    }
}
struct E;
void foo(void (E::*p1)(), void (E::*p2)())
{
    if (p1 == p2) // UndCC_Violation - 'E' is incomplete, so it is unknown if the pointers are to virtual members
    {
    }
}

// The following definition of E anywhere in the translation
// unit would make the above example UndCC_Valid
// struct E{ void f1(); void f2(); };
