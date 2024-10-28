#include <cstdint>

class C
{
public:
    C(int32_t a); // UndCC_Violation
};
class D
{
public:
    explicit D(int32_t a);          // Compliant
    D(const D &d);                  // Rule does not apply - copy constructor
    D(D &&d);                       // Rule does not apply - move constructor
    operator int32_t() const;       // UndCC_Violation(convops)
    explicit operator bool() const; // Compliant
};
class E
{
public:
    E(int32_t a, int32_t b = 0);    // UndCC_Violation - callable with one argument
    E(char a = 'a', int32_t b = 0); // UndCC_Violation - callable with one argument
    E(char a, char b);              // Rule does not apply - requires two arguments
};
void f(E e);
void g()
{
    f(0); // Implicit conversion from 0 to E
}
