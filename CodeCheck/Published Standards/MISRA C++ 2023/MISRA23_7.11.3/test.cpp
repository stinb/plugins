#include <cstdint>
#include <iostream>

void foo()
{
    extern int *f();
    if (f == nullptr) // UndCC_Violation
    {
    }
    if (&f != nullptr) // UndCC_Valid - no conversion
    {
        (f)(); // UndCC_Valid - no conversion
    }

    std::cout << std::boolalpha // UndCC_Valid - assignment to pointer-to-function type
              << f;             // UndCC_Violation - assignment is not to
                                // pointer-to-function type
    auto x = +f;                // UndCC_Violation
    void f1(double);
    void f1(uint32_t);
    auto selected = static_cast<void (*)(uint32_t)>(f1); // UndCC_Valid
    auto lam = []() {};
    void (*p)() = lam; // UndCC_Valid
    auto x2 = +lam;    // UndCC_Violation
    if (lam)           // UndCC_Violation
    {
    }
}
