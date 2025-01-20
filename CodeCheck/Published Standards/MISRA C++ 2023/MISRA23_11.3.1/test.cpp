#include <cstdint>
#include <cstddef>
#include <array>
#include <string>

void foo() noexcept
{
    const size_t size{10};
    wchar_t a1[size];             // UndCC_Violation
    std::array<wchar_t, size> a2; // UndCC_Valid
}

void bar(int a[10],    // Rule does not apply - pointer to int
         int (&b)[10], // Rule does not apply - reference to array
         int (*c)[10]) // Rule does not apply - pointer to array
{
}

struct S
{
    std::uint16_t a3[10]; // UndCC_Violation
};

using namespace std::literals;
const char s1[] = "abcd";   // UndCC_Valid by exception
char s2[] = "abcd";         // UndCC_Violation
const auto best = "abcd"; // UndCC_Valid
