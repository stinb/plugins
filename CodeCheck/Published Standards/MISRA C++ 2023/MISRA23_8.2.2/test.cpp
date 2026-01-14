#include <cstdint>

int32_t g();

void f1()
{
    (void)g(); // Compliant by exception
}

struct A
{
    A(char c);
};

struct B
{
};

void f1a(A x)
{
    auto const &a1 = x;
    A *a2 = (A *)&a1; // UndCC_Violation
    A *a3 = const_cast<A *>(&a1);
}

void f1b(B x)
{
    auto const &a1 = x;
    A *a2 = (A *)&a1; // UndCC_Violation
    // A *a3 = const_cast<A *>(&a1); // Ill-formed
}

void f2(int32_t x)
{
    auto i = A('c');     // Rule does not apply - constructor is called
    auto j = int8_t{42}; // Rule does not apply - use of curly braces
    auto k = int8_t(x);  // UndCC_Violation - does not construct an object
} // of class type
