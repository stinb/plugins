#include <cstdint>

struct A
{
    int32_t a;         // a is a subobject of *this
    int32_t &b;        // b is a reference, not a subobject of *this
    int32_t &geta1() & // UndCC_Valid - non-const-lvalue-ref-qualified
    {
        return a;
    }
    int32_t const &geta2() const & // UndCC_Valid - const-lvalue-ref-qualified and
    {
        return a;
    }
    int32_t geta2() && // this rvalue-ref overload exists
    {
        return a;
    }
    int32_t &getb() // Rule does not apply - b is not a subobject
    {
        return b;
    }
    A const *getMe1() const & { return this; } // UndCC_Valid
    void getMe1() const && = delete;           // - because this overload exists
    A &getMe2() { return *this; }              // UndCC_Violation - not ref-qualified
};

A createA();
// A call to the non-compliant getMe2 on a temporary results in immediate dangling
A &dangling = createA().getMe2();

class C
{
    C *f()
    {
        C *me = this;
        return me; // Indirectly designates 'this'
    }
};

struct Tmpl
{
    template <typename T>
    Tmpl const *f(T) const & { return this; } // Non-compliant only when instantiated for #3
    void f(int32_t) && = delete;              // #1
};
void bar(int32_t s32, int8_t s8)
{
    Tmpl tpl;
    tpl.f(s32); // #2
    tpl.f(s8);  // #3
}
