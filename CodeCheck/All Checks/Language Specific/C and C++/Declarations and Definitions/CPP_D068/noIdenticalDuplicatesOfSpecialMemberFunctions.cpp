// $Id: A12-7-1.cpp 271715 2017-03-23 10:13:51Z piotr.tanski $
#include <cstdint>
#include <utility>
class A
{
public:
    A() : x(0), y(0) {} // UndCC_Valid
    A(std::int32_t first, std::int32_t second) : x(first), y(second) {} // UndCC_Valid
    // -
    // anyway, such
    // a constructor
    // cannot be
    // defaulted.
    A(const A& oth) // UndCC_Violation
    : x(oth.x),
    y(oth.y) 
             // defined copy constructor
    {
    }
    A(A&& oth) // UndCC_Violation
    : x(std::move(oth.x)),
    y(std::move(
    oth.y)) 
            // defined move constructor
    {
    }
    ~A() // UndCC_Violation - equivalent to the implicitly defined destructor
    {
    }

private:
    std::int32_t x;
    std::int32_t y;
};
class B
{
public:
    B() {} // UndCC_Violation - x and y are not initialized
           // should be replaced with: B() : x{0}, y{0} {}
    B(std::int32_t first, std::int32_t second) : x(first), y(second) {} // UndCC_Valid
    B(const B&) =
    default; // UndCC_Valid - equivalent to the copy constructor of class A
    B(B&&) =
    default; // UndCC_Valid - equivalent to the move constructor of class A
    ~B() = default; // UndCC_Valid - equivalent to the destructor of class A

private:
    std::int32_t x;
    std::int32_t y;
};
class C
{
public:
    C() = default; // UndCC_Valid
    C(const C&) = default; // UndCC_Valid
    C(C&&) = default; // UndCC_Valid
};
class D
{
public:
    D() : ptr(nullptr) {}  // UndCC_Valid - this is not equivalent to what the
                           // implicitly defined default constructor would do
    D(C* p) : ptr(p) {}    // UndCC_Valid
    D(const D&) = default; // Shallow copy will be performed, user-defined copy
    // constructor is needed to perform deep copy on ptr variable
    D(D&&) = default; // ptr variable will be moved, so ptr will still point to
                      // the same object
    ~D() = default; // ptr will not be deleted, the user-defined destructor is
                    // needed to delete allocated memory

private:
    C* ptr;
};
class E // UndCC_Valid - special member functions definitions are not needed as
        // class E uses only implicit definitions
{
};
