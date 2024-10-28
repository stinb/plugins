// $Id: A13-1-3.cpp 289436 2017-10-04 10:45:23Z michal.szczepankiewicz $
#include <cstdint>
#include <iostream>
struct Cube
{
    unsigned long long int volume;
    constexpr explicit Cube(unsigned long long int v) : volume(v) {}
};
constexpr Cube operator"" _m3(unsigned long long int volume)
{
    return Cube(volume);   // UndCC_Violation
}
struct Temperature
{
    unsigned long long int kelvins;
    constexpr explicit Temperature(unsigned long long int k) : kelvins(k) {}
};
constexpr Temperature operator"" _K(unsigned long long int kelvins)
{
    return Temperature(kelvins);  // UndCC_Violation
}
static void SumDistances(std::int32_t distance)
{
    static std::int32_t overallDistance = 0;
    overallDistance += distance;
}
struct Distance
{
    long double kilometers;
    explicit Distance(long double kms) : kilometers(kms) {}
};
Distance operator"" _m(long double meters)
{
    SumDistances(meters); // UndCC_Violation - function has a side-effect
    return Distance(meters / 1000); // UndCC_Violation
}
void operator"" _print(const char *str)
{
    std::cout << str << '\n'; // UndCC_Violation - user-defined literal operator
    // does not perform conversion and has a
    // side-effect
}