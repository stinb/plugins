// $Id: A8-5-1.cpp 271696 2017-03-23 09:23:09Z piotr.tanski $
#include <cstdint>
#include <string>

class A
{
};

class B
{
};

class C : public virtual B, public A
{
public:
    C() : B(), A(), s() {} // UndCC_Valid

    

private:
    std::string s;
};

class D
{
};

class E
{
};

class F : public virtual A, public B, public virtual D, public E
{
public:
    F() : A(), D(), B(), E(), number1(0), number2(0U) {} // UndCC_Valid
    F(F const &oth) // UndCC_Violation
        : B(), E(), A(), D(), number1(oth.number1), number2(oth.number2)
    {
    } 
    // order of initialization

private:
    std::int32_t number1;
    std::uint8_t number2;
};
