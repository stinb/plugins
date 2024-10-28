// $Id: A10-3-4.cpp 289436 2017-10-04 10:45:23Z michal.szczepankiewicz $
class A
{
public:
    virtual A &operator=(A const &oth) = 0;  // UndCC_Violation
    virtual A &operator+=(A const &rhs) = 0; // UndCC_Violation
};
class B : public A
{
public:
    B &operator=(A const &oth) override // It needs to take an argument of type
    // A& in order to override
    {
        return *this;
    }
    B &operator+=(A const &oth) override // It needs to take an argument of
    // type A& in order to override
    {
        return *this;
    }
    B &operator-=(B const &oth) // UndCC_Valid
    {
        return *this;
    }
};
class C : public A
{
public:
    C &operator=(A const &oth) override // It needs to take an argument of type
    // A& in order to override
    {
        return *this;
    }
    C &operator+=(A const &oth) override // It needs to take an argument of
    // type A& in order to override
    {
        return *this;
    }
    C &operator-=(C const &oth) // UndCC_Valid
    {
        return *this;
    }
};
// class D : public A
//{
// public:
// D& operator=(D const& oth) override // Compile time error - this method
// does not override because of different
// signature
// {
// return *this;
// }
// D& operator+=(D const& oth) override // Compile time error - this method
// does not override because of different
// signature
// {
// return *this;
// }
//};
void Fn() noexcept
{
    B b;
    C c;
    b = c;  // Calls B::operator= and accepts an argument of type C
    b += c; // Calls B::operator+= and accepts an argument of type C
    c = b;  // Calls C::operator= and accepts an argument of type B
    c += b; // Calls C::operator+= and accepts an argument of type B
    // b -= c; // Compilation error, because of types mismatch. Expected
    // behavior
    // c -= b; // Compilation error, because of types mismatch. Expected
    // behavior

    B b2;
    C c2;
    b -= b2;
    c -= c2;
}
