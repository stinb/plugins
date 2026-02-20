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
// Passing to function with non-const pointer parameter
void memory_u32_cpy(void *dst, const void *src, int size);
void read_only(const void *src, int size);

int f5(int8_t *const p_payload) // UndCC_Valid - passed to non-const pointer param
{
    memory_u32_cpy(p_payload, nullptr, 10);
    return 0;
}

int f6(int8_t *p_data) // UndCC_Violation - passed only to const pointer param
{
    read_only(p_data, 10);
    return 0;
}

void nested_call_helper(int *dst, int val);
int f7(int *p,  // UndCC_Valid - passed to non-const pointer param
       int *q)  // UndCC_Violation - not modified, only dereferenced
{
    nested_call_helper(p, *q);
    return 0;
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
