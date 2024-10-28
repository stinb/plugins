// AUTOSAR C++ 2014 - A5-1-6

#include <cstdint>
void Fn() noexcept
{
    auto lambda1 = []() -> std::uint8_t {
        std::uint8_t ret = 0U;
        // ...
        return ret;
    };
    auto lambda2 = []() { // UndCC_Violation - returned type is not specified
        // ...
        return 0U;
    };
    auto x = lambda1();  // Type of x is std::uint8_t
    auto y = lambda2();  // What is the type of y?
}
