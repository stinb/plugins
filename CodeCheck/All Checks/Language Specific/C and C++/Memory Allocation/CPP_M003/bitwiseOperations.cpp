// $Id: A12-0-2.cpp 305629 2018-01-29 13:29:25Z piotr.serwa $
#include <cstdint>
#include <cstring>

class A
{
public:
    A() = default;
    A(uint8_t c, uint32_t i, int8_t d, int32_t h) : c(c), i(i), d(d), h(h) {}

    bool operator==(const A &rhs) const noexcept
    {
        return c == rhs.c && i == rhs.i && d == rhs.d && h == rhs.h;
    }

private:
    uint8_t c;
    uint32_t i;
    int8_t d;
    int32_t h;
};

int main(void)
{
    A noninit;

    // setting field c
    std::memset(&noninit, 3, 1); // UndCC_Violation
    // setting field i
    std::memset(((uint8_t *)&noninit) + sizeof(uint8_t) + 3, 5, 1); // UndCC_Violation

    A init(3, 5, 7, 9); // UndCC_Valid

    if (noninit == init) // UndCC_Valid
    {
    }

    if (0 == std::memcmp(&noninit, &init, sizeof(init))) // UndCC_Violation
    { 
    }

    return 0;
}
