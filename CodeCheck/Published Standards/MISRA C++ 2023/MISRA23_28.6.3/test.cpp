#include <string>
#include <cstdint>

size_t a(std::string s1)
{
    std::string s2 = std::move(s1);
    return s1.size(); // UndCC_Violation - s1 has potentially
} // moved-from state

size_t b(std::string s1)
{
    std::string s2 =
        static_cast<std::string &&>(s1); // Equivalent to std::move
    return s1.size();                    // UndCC_Violation - s1 has potentially
}

void c(std::string s1)
{
    std::string s2 = std::move(s1);
    std::string s3 = s1; // UndCC_Violation - s1 has potentially
} // moved-from state

template <typename T>
void bar(T &t);

template <typename T>
void foo(T &&t)
{
    bar(std::forward<T>(t));
    ++t; // UndCC_Violation - std::forward leaves t
} // in a potentially moved-from state

struct X
{
    std::string s;
};

void f(X &x)
{
    X y(std::move(x)); // UndCC_Violation - lvalue reference
                       // parameter left in potentially moved-
} // from state when function returns

void g(X x)
{
    X y;
    y = std::move(x); // UndCC_Valid - no more uses of x
}

void h(X x)
{
    X y;
    y = std::move(x);
    x = X{}; // UndCC_Valid - assigns to potentially
} // moved-from object

void d1(std::string const &, int32_t);
int32_t d2(std::string &&);

void d3(std::string s)
{
    d1(s, d2(std::move(s))); // UndCC_Violation
}
