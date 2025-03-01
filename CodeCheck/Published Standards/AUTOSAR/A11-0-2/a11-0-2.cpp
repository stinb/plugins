// $Id: A11-0-2.cpp 289436 2017-10-04 10:45:23Z michal.szczepankiewicz $
#include <cstdint>
struct A // UndCC_Valid
{
    std::int32_t x;
    double y;
};
struct B // UndCC_Valid
{
    std::uint8_t x;
    A a;
};
struct C // UndCC_Valid
{
    float x = 0.0f;
    std::int32_t y = 0;
    std::uint8_t z = 0U;
};
struct D // UndCC_Violation
{
public:
    std::int32_t x;

protected:
    std::int32_t y;

private:
    std::int32_t z;
};
struct E // UndCC_Violation
{
public:
    std::int32_t x;
    void Fn() noexcept {}

private:
    void F1() noexcept(false) {}
};
struct F : public D // UndCC_Violation
{
};
