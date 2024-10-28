// $Id: A7-3-1.cpp 312801 2018-03-21 16:17:05Z michal.szczepankiewicz $
#include <cstdint>

class Base
{
public:
    void P(uint32_t) {}
    virtual void V(uint32_t) {}
    virtual void V(double) {}
};

class NonCompliant : public Base
{
public:
    // hides P(uint32_t) when calling from the
    // derived class
    void P(double) {} // UndCC_Violation
    // hides V(uint32_t) when calling from the
    // derived class
    void V(double) override {} // UndCC_Violation
};

class Compliant : public Base
{
public:
    // both P(uint32_t) and P(double) available
    // from the derived class
    using Base::P;
    void P(double) {}
    // both P(uint32_t) and P(double)
    using Base::V;
    void V(double) override {}
};

void F1()
{
    NonCompliant d{};
    d.P(0U); // D::P (double) called
    Base &b{d};
    b.P(0U); 
    d.V(0U); // D::V (double) called
    b.V(0U); 
}

void F2()
{
    Compliant d{};
    NonCompliant n{};
    d.P(0U); // UndCC_Valid::P (uint32_t) called
    Base &b{d};
    Base &b2{n};
    b.P(0U);  // UndCC_Valid::P (uint32_t) called
    d.V(0U);  // UndCC_Valid::V (uint32_t) called
    b.V(0U);  // UndCC_Valid::V (uint32_t) called
    b2.P(0U); 
    b2.V(0U); 
}

namespace NS
{
    void F(uint16_t) {}
}

// includes only preceding declarations into
// the current scope
using NS::F;
namespace NS
{
    void F(uint32_t) {} // UndCC_Violation
}

void B(uint32_t b)
{
    
    // in this scope
    F(b);
}
