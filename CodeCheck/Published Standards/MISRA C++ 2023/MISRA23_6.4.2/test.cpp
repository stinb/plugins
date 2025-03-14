#include <cstdint>

class Base
{
public:
    void f1(int32_t i);
    void f2(int32_t i);
    virtual Base *f3(char c);
    void f4(char c);
    void f5(int32_t i);
    void f5(char c);
};

class Derived : public Base
{
public:
    Derived &operator=(Derived const &) &; // UndCC_Valid - does not conceal Base::operator=

    void f1(float f); // UndCC_Violation - Derived::f1 conceals Base::f1

    using Base::f2;   // UndCC_Valid - Base::f2 is not concealedIntroduces Base::f2( int32_t ) overload
    void f2(float f); // Using declaration means this overload does not conceal

    Derived *f3(char const c) override; // overrides Base::f3( char )
    void f3(int32_t i);

    void f4(char c);    // UndCC_Violation - Base::f4 is concealed
    void f4(int32_t i); // UndCC_Violation - Not an override

    // Base::f5( int32_t ) is concealed
    using Base::f5;     // Introduces Base::f5( char ), not Base::f5( int32_t ) as
    void f5(int32_t i); // UndCC_Violation - this function has the same signature
};

class PrivateDerived : private Base
{
public:
    void f1(float f); // UndCC_Valid - Base inherited privately
};
