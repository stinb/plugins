// AUTOSAR

#include <cstdint>
#include <functional>

std::function<std::int32_t()> F()
{
    std::int32_t i = 12; // UndCC_Violation
    return ([&i]() -> std::int32_t {
        i = 100;
        return i;
    }); // The violation here is reported above.
}

std::function<std::int32_t()> G()
{
    std::int32_t i = 12; // UndCC_Valid
    return ([i]() mutable -> std::int32_t { return ++i; });
}

void Fn()
{
    auto lambda1 = F();
    std::int32_t i = lambda1(); // Undefined behavior
    auto lambda2 = G();
    i = lambda2(); // lambda2() returns 13
}
