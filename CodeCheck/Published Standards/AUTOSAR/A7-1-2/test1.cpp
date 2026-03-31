// AUTOSAR C++ 2014

int Pow1(int number)
{
    return (number * number);
}
constexpr int Pow2(
    int number) // Possible compile-time computing
                // because of constexpr specifier
{
    return (number * number);
}

void Fn()
{
    constexpr short i1 = 20;           // UndCC_Valid, evaluated at compile-time
    const short i2 = 20;               // UndCC_Violation, possible run-time evaluation
    int twoSquare = Pow1(2);           // UndCC_FalseNeg, possible run-time evaluation
    const int threeSquare = Pow1(3);   // UndCC_FalseNeg, possible run-time evaluation
    // static_assert(threeSquare == 9, "pow1(3) did not succeed."); // Value
    // can not be static_assert-ed
    constexpr int fiveSquare = Pow2(5); // UndCC_Valid, evaluated at compile time
    static_assert(fiveSquare == 25,
                  "pow2(5) did not succeed."); // UndCC_Valid, constexpr
                                               // evaluated at compile time
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
