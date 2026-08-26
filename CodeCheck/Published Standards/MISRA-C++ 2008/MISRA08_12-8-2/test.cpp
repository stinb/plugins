#include <cstdint>

// Abstract class with a public copy assignment operator
class B1
{
public:
    B1();
    virtual void f() = 0;
    B1 &operator=(B1 const &rhs); // UndCC_Violation
    int32_t getKind() const { return kind; }

private:
    int32_t kind;
};

// Concrete derived class - rule does not apply
class D1 : public B1
{
public:
    virtual void f() {}
    D1 &operator=(D1 const &rhs); // UndCC_Valid - D1 is not abstract

private:
    int32_t member;
};

// Abstract class with a protected copy assignment operator
class B2
{
public:
    B2();
    virtual void f() = 0;

protected:
    B2 &operator=(B2 const &rhs); // UndCC_Valid - protected

private:
    int32_t kind;
};

// Abstract class with a private copy assignment operator
class B3
{
public:
    virtual void f() = 0;

private:
    B3 &operator=(B3 const &rhs); // UndCC_Valid - private
};

// Abstract class that is still abstract despite having a base
class B4 : public B1
{
public:
    virtual void g() = 0;
    B4 &operator=(B4 const &rhs); // UndCC_Violation

private:
    int32_t member;
};

// Non-copy assignment operator in an abstract class - rule does not apply
class B5
{
public:
    virtual void f() = 0;
    B5 &operator=(int32_t rhs); // UndCC_Valid - not a copy assignment
};

// Concrete class with a public copy assignment operator
class C1
{
public:
    void f() {}
    C1 &operator=(C1 const &rhs); // UndCC_Valid - not abstract
};

// Abstract class with a deleted copy assignment operator
class B6
{
public:
    virtual void f() = 0;
    B6 &operator=(B6 const &rhs) = delete; // UndCC_Valid - deleted, cannot be invoked
};

// Abstract only through an inherited, unoverridden pure virtual
class B7 : public B1
{
public:
    B7 &operator=(B7 const &rhs); // UndCC_Violation
};

// Copy assignment taking its parameter by value
class B8
{
public:
    virtual void f() = 0;
    B8 &operator=(B8 rhs); // UndCC_Violation
};

// Class template with a public copy assignment operator
template <typename T>
class B9
{
public:
    virtual void f() = 0;
    B9 &operator=(B9 const &rhs); // UndCC_Violation
};

// Abstract through a pure virtual destructor
class B10
{
public:
    virtual ~B10() = 0;
    B10 &operator=(B10 const &rhs); // UndCC_Violation
};

// Move assignment is not a copy assignment operator
class B11
{
public:
    virtual void f() = 0;
    B11 &operator=(B11 &&rhs); // UndCC_Valid - move assignment
};

// Copy assignment defined inline in the class
class B12
{
public:
    virtual void f() = 0;
    B12 &operator=(B12 const &rhs) { return *this; } // UndCC_Violation
};

// Abstract only through a dependent base: not decidable before instantiation,
// so the parser kinds XJ as a plain class template
template <typename T>
class XI
{
public:
    virtual void f() = 0;
};

template <typename T>
class XJ : public XI<T>
{
public:
    XJ &operator=(XJ const &rhs); // UndCC_FalseNeg
};

// Copy assignment whose parameter is spelled through a typedef or using alias
class TA
{
public:
    typedef TA Self;
    virtual void f() = 0;
    TA &operator=(Self const &rhs); // UndCC_Violation
};

class TB
{
public:
    using Self = TB;
    virtual void f() = 0;
    TB &operator=(Self const &rhs); // UndCC_Violation
};

class TC;
typedef TC TCAlias;
class TC
{
public:
    virtual void f() = 0;
    TC &operator=(TCAlias const &rhs); // UndCC_Violation
};

// A different class as the parameter is not a copy assignment operator
class TDExtra
{
};
class TD
{
public:
    virtual void f() = 0;
    TD &operator=(TDExtra const &rhs); // UndCC_Valid
};

// A pointer resolves to its pointee, so these are not copy assignments
class PP
{
public:
    virtual void f() = 0;
    PP &operator=(PP const *rhs); // UndCC_Valid - pointer parameter
};

class PQ
{
public:
    virtual void f() = 0;
    PQ &operator=(PQ *const &rhs); // UndCC_Valid - reference to pointer
};

class PR
{
public:
    virtual void f() = 0;
    PR &operator=(PR (&arr)[3]); // UndCC_Valid - reference to array
};
