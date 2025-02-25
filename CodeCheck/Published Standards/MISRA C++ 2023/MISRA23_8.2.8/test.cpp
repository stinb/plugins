#include <cstdint>

struct S;

void f1(S *s)
{
    auto p0 = reinterpret_cast<std::uintptr_t>(s); // UndCC_Valid
    auto p1 = reinterpret_cast<unsigned long>(s);  // UndCC_Violation
    using hashPtr_t = std::uintptr_t;
    auto p2 = reinterpret_cast<hashPtr_t>(s); // UndCC_Violation
}

template <typename T>
void f2(S *s)
{
    auto p = reinterpret_cast<T>(s); // UndCC_Violation - T is not explicitly
} // std::uintptr_t

template void f2<std::uintptr_t>(S *s);