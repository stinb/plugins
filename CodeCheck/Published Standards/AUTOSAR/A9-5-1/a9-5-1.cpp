// $Id: A9-5-1.cpp 305588 2018-01-29 11:07:35Z michal.szczepankiewicz $

#include <cstdint>
// UndCC_Valid
struct Tagged
{
    enum class TYPE
    {
        UINT,
        FLOAT
    };
    union
    {
        uint32_t u;
        float f;
    };
    TYPE which;
};

int main(void)
{
    Tagged un;

    un.u = 12;
    un.which = Tagged::TYPE::UINT;

    un.u = 3.14f;
    un.which = Tagged::TYPE::FLOAT;

    return 0;
}

union S // UndCC_Violation
{
    std::int32_t n;     // occupies 4 bytes
    std::uint16_t s[2]; // occupies 4 bytes
    std::uint8_t c;     // occupies 1 byte
};