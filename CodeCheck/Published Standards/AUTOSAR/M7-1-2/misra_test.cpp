#include <cstdint>
#include <vector>

void f1(int8_t *p1,       // UndCC_Valid - *p1 modified
        const int8_t *p2, // UndCC_Valid - *p2 not modified, but is const
        int8_t *p3,       // UndCC_Violation - *p3 not modified, no const
        int8_t *const p4, // UndCC_Violation - *p4 not modified, no const
        int8_t a[3])      // UndCC_Violation - 'a' decays to int8_t *
{
    *p1 = *p2 + *p3 + *p4;
}
auto &f2(int32_t &i,  // UndCC_Valid
         int32_t &&j, // Rule does not apply - rvalue reference
         int32_t &)   // Rule does not apply - unnamed parameter
{
    return i; // Assigning to non-const reference
}
auto f3(std::vector<int32_t> &x) // UndCC_Valid - even though x.begin has an
{                                // equivalent const overload
    return x.begin();            // Non-const member function
}
auto f4(std::vector<int32_t> &x) // UndCC_Violation
{
    return x.cbegin(); // Const member function
}
template <typename T>
struct A
{
    void foo(T &t,       // Rule does not apply - in template scope
             int32_t &i) // Rule does not apply - in template scope
    {
        t.f(i); // t and/or i may or may not be modified,
    } // depending on the signature of T::f
};
