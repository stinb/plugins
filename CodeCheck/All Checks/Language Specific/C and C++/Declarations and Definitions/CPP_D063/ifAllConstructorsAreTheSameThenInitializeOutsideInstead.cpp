// $Id: A12-1-3.cpp 291949 2017-10-19 21:26:22Z michal.szczepankiewicz $
#include <cstdint>
#include <string>
class A
{
public:
    A() : x(0), y(0.0F), str() // UndCC_Violation
    {
    }
    // ...

private:
    std::int32_t x;
    float y;
    std::string str;
};
class B
{
public:
    // ...

private:
    std::int32_t x = 0;   // UndCC_Valid
    float y = 0.0F;       // UndCC_Valid
    std::string str = ""; // UndCC_Valid
};
class C
{
public:
    C() : C(0, 0.0F, decltype(str)()) // UndCC_Valid
    {
    }
    C(std::int32_t i, float f, std::string s) : x(i), y(f), str(s) // UndCC_Valid
    {
    }
    // ...

private:
    std::int32_t x = // UndCC_Violation
        0; 
    // class with user input
    float y = 0.0F; // UndCC_Violation - there’s a constructor that initializes C
    // class with user input
    std::string str = ""; // UndCC_Violation - there’s a constructor that
    // initializes C class with user input
};
