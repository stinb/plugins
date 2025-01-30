#include <cstdint>

void foo()
{
    uint8_t u8a;

    auto x = +u8a;      // UndCC_Violation - triggers promotion to int
    auto pf = +[]() {}; // UndCC_Violation - pf is a void(*)()

    x = +1; // UndCC_Violation
    x =+ 1; // UndCC_Violation - unary +, not +=
}

enum A : uint8_t
{
    one
};
enum B : uint8_t
{
    two
};

uint8_t operator+(B b) { return b; }
auto a = +one;           // UndCC_Violation
auto b = +two;           // Rule does not apply
auto c = operator+(two); // Rule does not apply
