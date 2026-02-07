#include <string>

void f1(std::string &);  // #1
void f1(std::string &&); // #2

template <typename T1, typename T2>
void f2(T1 &&t1, T2 &t2)
{
    f1(t1);                   // UndCC_Violation - calls #1
    f1(std::forward<T1>(t1)); // UndCC_Valid - calls #1 (for #4) or #2 (for #3)
    f1(std::forward<T2>(t2)); // UndCC_Violation - calls #2
    f1(std::forward<T2>(t1)); // UndCC_Violation - wrong template parameter
    f1(std::move(t1));        // UndCC_Violation - calls #2
    f1(std::move(t2));        // Rule does not apply - calls #2
    auto lambda = [](auto &&t)
    { f1(t); }; // UndCC_Violation - calls #1
}

void f3()
{
    std::string s;
    f2(std::string{"Hello"}, s); // #3
    f2(s, s);                    // #4
}

template <typename T>
struct A
{
    void foo(T &&t)
    {
        std::move(t); // Rule does not apply - not a forwarding reference
    }
};