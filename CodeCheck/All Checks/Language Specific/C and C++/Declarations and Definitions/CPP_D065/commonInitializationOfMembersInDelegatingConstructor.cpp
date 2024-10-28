// $Id: A12-1-5.cpp 289436 2017-10-04 10:45:23Z michal.szczepankiewicz $

#include <cstdint>

class A
{
public:
    A(std::int32_t x, std::int32_t y) : x(x + 8), y(y) {} // UNDCC_Valid
    explicit A(std::int32_t x) : A(x, 0) {}               // UNDCC_Valid

private:
    std::int32_t x;
    std::int32_t y;
};

class B
{
public:
    B(std::int32_t x, std::int32_t y) : x(x + 8), y(y) {}
    explicit B(std::int32_t x) : x(x + 8), y(0) {}        // UNDCC_Violation

private:
    std::int32_t x;
    std::int32_t y;
};
