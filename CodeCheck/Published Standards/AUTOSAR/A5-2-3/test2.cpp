// AUTOSAR C++ 2014

#include <cstdint>
void F1(const char* str) noexcept(false)
{
    *(const_cast<char*>(str)) = '\0';  // UndCC_Violation - const qualification removed
}
class C
{
public:
    explicit C(std::int32_t) {}
};
void F2() noexcept(false)
{
    C const a1 = C(10);
    C* a2 = const_cast<C*>(&a1);  // UndCC_Violation - const qualification removed
    C* a3 = (C*)&a1;              // UndCC_Violation - const qualification removed
}
extern volatile std::int32_t* F3() noexcept;
void F4() noexcept
{
    volatile std::int32_t* ptr1 = F3();
    // ...
    std::int32_t* ptr2 = const_cast<std::int32_t*>(ptr1);  // UndCC_Violation - volatile qualification removed
    // ...
    std::int32_t* ptr3 = (std::int32_t*)ptr1;  // UndCC_Violation - volatile qualification removed
}
