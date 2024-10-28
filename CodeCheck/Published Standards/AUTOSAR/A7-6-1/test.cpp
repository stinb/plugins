// $Id: A7-6-1.cpp 305629 2018-01-29 13:29:25Z piotr.serwa $
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
} // UNDCC_Violation

[[noreturn]] void g(int i) // UNDCC_Valid
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
