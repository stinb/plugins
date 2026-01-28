#include <iostream>

struct A
{
    int i{42};
    virtual ~A() = default;
};

void f()
{
    std::printf("hello %d", A{}); // UndCC_FalseNeg, std library cannot be checked
}

struct two
{
    char x[2];
};

two test(int);
char test(...);

template <typename T>
constexpr bool isIntCompatible(T x)
{
    if constexpr (sizeof(test(x)) == 1) // UndCC_Valid - unevaluated context
    {
        return false; // Overload resolution -> test( ... )
    }
    else
    {
        return true; // Overload resolution -> test( int )
    }
}

void f(struct A a)
{
    test(a); // UndCC_Violation
}