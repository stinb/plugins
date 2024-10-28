// $Id: A5-1-1.cpp 289436 2017-10-04 10:45:23Z michal.szczepankiewicz $
#include <array>
#include <cstdint>
#include <iostream>
#include <stdexcept>
namespace
{
    const std::int32_t maxIterations = 10;   // UndCC_Valid - assignment
    const char* const loopIterStr = "iter "; // UndCC_Valid - assignment
    const char separator = ':';              // UndCC_Valid - assignment
}
void F1() noexcept
{
    for (std::int32_t i = 0; i < 10; ++i) // UndCC_Violation
    {
        std::cout << "iter " << i << ':' << '\n'; // UndCC_Valid by exception
    }

    for (std::int32_t i = 0; i < maxIterations; ++i) // UndCC_Valid
    {
        std::cout << loopIterStr << i << separator << '\n'; // UndCC_Valid
    }

    for (std::int32_t i = 0; i < maxIterations; ++i) // UndCC_Valid
    {
        std::cout << "iter " << i << ':' << '\n'; // UndCC_Valid by exception
    }
}
void F2()
{
    // ...
    throw std::logic_error("Logic Error"); // UndCC_Valid
    // initialization of exception object
}
class C
{
  public:
    C() : x(0), y(nullptr) // UndCC_Valid - initialization
    {
    }
    C(std::int8_t num, std::int32_t* ptr) : x(num), y(ptr) {}

  private:
    std::int8_t x;
    std::int32_t* y;
};
static std::int32_t* globalPointer = nullptr; // UndCC_Valid - assignment
void F3() noexcept
{
    C c1;
    // ...
    C c2(0, globalPointer); // UndCC_Valid - initialization of C object
}
std::int32_t F4(std::int32_t x, std::int32_t y) noexcept
{
    return x + y;
}
void F5() noexcept
{
    std::int32_t ret = F4(2, 5); // UndCC_Violation
    // ...
    std::int32_t x = 2;
    std::int32_t y = 5;
    ret = F4(x, y); // UndCC_Valid

    std::array<std::int8_t, 5> arr{{1, 2, 3, 4, 5}}; // UndCC_Valid
}
