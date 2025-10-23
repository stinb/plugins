// AUTOSAR C++ 2014

#include <cstdint>
#include <exception>

class PositiveInputException : public std::exception {};

[[noreturn]] void f(int i)
{
    if (i > 0)
    {
        throw PositiveInputException();
    }
    //undefined behaviour for non-positive i
} // UndCC_Violation

[[noreturn]] void g(int i) // UndCC_Valid
{
    if (i > 0)
    {
        throw "Received positive input";
    }

    while(1)
    {
        //do processing
    }
}
