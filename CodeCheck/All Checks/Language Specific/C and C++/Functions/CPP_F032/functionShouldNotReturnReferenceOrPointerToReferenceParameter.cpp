// MISRA C++ 2008 7-5-3 example: a function shall not return a reference or a
// pointer to a parameter passed by reference OR const reference. AUTOSAR A7-5-1
// only covers reference-to-const, so the non-const cases are violations only
// under the broad option (include_nonconst).
#include <cstdint>

std::int32_t* fn1(std::int32_t& x) // UndCC_Violation(broad) - non-const reference parameter
{
    return (&x);
}

std::int32_t* fn2() // UndCC_Valid - not a reference parameter
{
    std::int32_t i = 0;
    return fn1(i);
}

const std::int32_t* fn3(const std::int32_t& x) // UndCC_Violation - const reference parameter
{
    return (&x);
}

std::int32_t& fn4(std::int32_t& x) // UndCC_Violation(broad) - non-const reference parameter
{
    return (x);
}

const std::int32_t& fn5(const std::int32_t& x) // UndCC_Violation - const reference parameter
{
    return (x);
}

std::int32_t byValue(std::int32_t x) // UndCC_Valid - parameter passed by value
{
    return x;
}
