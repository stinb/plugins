// AUTOSAR C++ 2014

#include <cstdint>
std::int32_t Pow1(std::int32_t number)
{
    return (number * number);
}
constexpr std::int32_t Pow2(
    std::int32_t number) // Possible compile-time computing
                         // because of constexpr specifier
{
    return (number * number);
}
void Fn()
{
    constexpr std::int16_t i1 = 20; // UndCC_Valid, evaluated at compile-time
    const std::int16_t i2 = 20; // UndCC_Violation, possible run-time evaluation
    std::int32_t twoSquare = Pow1(2); // UndCC_Violation, possible run-time evaluation
    const std::int32_t threeSquare = Pow1(3); // UndCC_Violation, possible run-time evaluation
    // static_assert(threeSquare == 9, "pow1(3) did not succeed."); // Value
    // can not be static_assert-ed
    constexpr std::int32_t fiveSquare = Pow2(5); // UndCC_Valid, evaluated at compile time
    static_assert(fiveSquare == 25,
                  "pow2(5) did not succeed."); // UndCC_Valid, constexpr
                                               // evaluated at compile time
    // constexpr std::int32_t int32Max =
    // std::numeric_limits<std::int32_t>::max() + 1; //
    // UndCC_Valid - compilation error due to
    // compile-time evaluation (integer overflow)
}
class A
{
  public:
    static constexpr double pi = 3.14159265; // UndCC_Valid - value of PI can be
                                             // determined in compile time

    // constexpr double e = 2.71828182; // constexprs need
    // to be static members, compilation error

    constexpr A() = default; // UndCC_Valid
};
