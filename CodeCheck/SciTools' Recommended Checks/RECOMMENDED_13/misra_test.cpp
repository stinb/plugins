#include <cstdint>

struct Foo
{
    int32_t m1() // UndCC_Violation(0) Public - rule does not apply for MISRA
    {
        return -1;
    }
    static int32_t m2() // UndCC_Violation(0) Class scope - rule does not apply for MISRA
    {
        return 42;
    }
    Foo()
    {
        m3();
    }

private:
    void m3() {} // UndCC_Valid - called
    void m4() {} // UndCC_Violation - not used
    void m5() {} // UndCC_Valid - used by a friend
    friend void (*f4())();

protected:
    void m6() {} // UndCC_Violation(0)
};
static void f1() {} // UndCC_Violation - not used
namespace
{
    void f2() {} // UndCC_Violation - not used
}
static void f3() {} // UndCC_Valid - address taken in f4()
void (*f4())()      // UndCC_Violation(0) Rule does not apply for MISRA - visibility not limited
{
    Foo bar;
    bar.m5();
    return &f3;
}
namespace A
{
    struct C1
    {
    };
    static void swap(C1 &, C1 &); // UndCC_Valid - overload set for call in f5
}
namespace B
{
    struct C2
    {
    };
    static void swap(C2 &, C2 &); // UndCC_FalseNeg
}
namespace
{
    template <typename T>
    void swap(T &, T &); // UndCC_Valid - overload set for call in f5
}
void f5(A::C1 c1, A::C1 c2) // UndCC_Violation(0) Rule does not apply for MISRA - visibility not limited
{
    swap(c1, c2);
}
