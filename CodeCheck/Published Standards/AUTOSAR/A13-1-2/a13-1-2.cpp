// $Id: A13-1-2.cpp 289436 2017-10-04 10:45:23Z michal.szczepankiewicz $
constexpr long double operator"" _m(long double meters) // UndCC_Valid
{
    // Implementation
    return meters;
}
constexpr long double operator"" _kg(long double kilograms) // UndCC_Valid
{
    // Implementation
    return kilograms;
}
constexpr long double operator"" m(long double meters) // UndCC_Violation
{
    // Implementation
    return meters;
}
constexpr long double operator"" kilograms( // UndCC_Violation
    long double kilograms) 
{
    // Implementation
    return kilograms;
}
void Fn()
{
    long double weight = 20.0_kg;
    long double distance = 204.8_m;
}
