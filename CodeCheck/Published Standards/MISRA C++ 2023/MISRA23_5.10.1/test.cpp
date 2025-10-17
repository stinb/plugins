#include <cstdio> // UndCC_Valid - even though it introduces FILE
#include <functional>

namespace std
{
    struct A // UndCC_Violation - defined within namespace std
    {
        int id; // UndCC_Violation - defined within namespace std
    };

    template <>
    struct hash<A> // Rule does not apply
    {
        size_t operator()(const A &x) const;
    };
}

void operator""_km(long double);    // UndCC_Valid - will be called for 1.0_km
void operator"" mil(long double);   // UndCC_Violation - user-defined literal suffixes shall start with _
double operator""_Bq(long double);  // UndCC_Valid
double operator"" _Bq(long double); // UndCC_Violation - _Bq is preceded by a space, making it a reserved identifier

// error in strict parser
// int32_t i﴾ = 2; // Non-compliant - character \ufd3e (even though it may compile)

#define identity(a) a // UndCC_Violation - shall be in uppercase

void f()
{
    auto _i = 0; // UndCC_Violation - using a leading _, even at local scope, is prohibited
}

namespace std42
{
    inline namespace a // UndCC_Violation - defined within namespace stdN
    {
        int i; // UndCC_Violation - defined within namespace stdN
    }
}

auto final = 42; // UndCC_Violation