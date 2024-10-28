// $Id: A8-5-2.cpp 289436 2017-10-04 10:45:23Z michal.szczepankiewicz $
#include <cstdint>
#include <initializer_list>

void F1() noexcept
{
    std::int32_t x1 = // UndCC_Violation
        7.9; 
    // std::int32_t y {7.9}; // UndCC_Valid - compilation error, narrowing
    std::int8_t x2{300};   // UndCC_Valid
    std::int8_t x3 = {50}; // UndCC_Violation - std::int8_t x3 {50} is equivalent
    // and more readable
    std::int8_t x4 = // UndCC_Violation
        1.0;              
    std::int8_t x5 = 300; // UndCC_Violation - narrowing occurs implicitly
    std::int8_t x6(x5);   // UndCC_Violation
    bool b{true};
}
class A
{
public:
    A(std::int32_t first, std::int32_t second) : x{first}, y{second} {}

private:
    std::int32_t x;
    std::int32_t y;
};
struct B
{
    std::int16_t x;
    std::int16_t y;
};
class C
{
public:
    C(std::int32_t first, std::int32_t second) : x{first}, y{second} {}
    C(std::initializer_list<std::int32_t> list) : x{0}, y{0} {}

private:
    std::int32_t x;
    std::int32_t y;
};
void F2() noexcept
{
    A a1{1, 5};    // UndCC_Valid - calls constructor of class A
    A a2 = {1, 5}; // UndCC_Violation - calls a default constructor of class A
    // and not copy constructor or assignment operator.
    A a3(1, 5); // UndCC_Violation
    B b1{5, 0}; // UndCC_Valid - struct members initialization
    C c1{2, 2}; // UndCC_Valid - C(std::initializer_list<std::int32_t>)
    // constructor is
    // called
    C c2(2, 2); // UndCC_Valid by exception - this is the only way to call
    // C(std::int32_t, std::int32_t) constructor
    C c3{{}}; // UndCC_Valid - C(std::initializer_list<std::int32_t>) constructor
    // is
    // called with an empty initializer_list
    C c4({2, 2}); // UndCC_Valid by exception -
                  // C(std::initializer_list<std::int32_t>)
    // constructor is called
};
template <typename T, typename U>
void F1(T t, U u) noexcept(false)
{
    std::int32_t x = 0;
    T v1(x); // UndCC_Violation
    T v2{x}; // UndCC_Valid - v2 is a variable
    
    // Compilation error
};
void F3() noexcept
{
    F1(0, "abcd"); // Compile-time error, cast from const char* to int
}
