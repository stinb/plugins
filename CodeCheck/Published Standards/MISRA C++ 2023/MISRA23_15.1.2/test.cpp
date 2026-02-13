#include <cstdint>

class A
{
public:
    A() {} // Rule does not apply - no base classes
};
class B : public A
{
public:
    B() {} // UndCC_Valid by exception
};
class V
{
public:
    V() {}                 // Rule does not apply - no base classes
    V(int32_t i) : i(i) {} // Rule does not apply - no base classes
private:
    int32_t i = 0;
};
class C1 : public virtual V
{
public:
    C1() : V{21} {} // UndCC_Valid
};
class C2 : public virtual V
{
public:
    C2() : V{42} {} // UndCC_Valid
};

class Dn : public C1, public C2
{
public:
    Dn() {} // UndCC_Violation
};

class Dc : public C1, public C2
{
public:
    Dc() : V{}, C1{}, C2{} {} // UndCC_Valid - V::i == 0
};

struct E
{
    int32_t i;
    int32_t j;
};
class F : public E
{
public:
    F(int32_t val) : E{val} // UndCC_Valid - E is initialized by aggregate
    {
    } // initialization, with E::j initialized to 0
    F() : F(0) {} // UndCC_Valid - delegates to the other constructor
};
class G : public A // Rule does not apply - no user-provided
{                  // constructor
};
class H : public A, public V // Rule does not apply - no user-provided
{                            // constructor
public:
    using V::V; // Subobject 'A' implicitly initialized
};
