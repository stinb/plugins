#include <cstdint>

void f(int32_t a);
void f(uint32_t a);

void foo()
{
    uint8_t a = -1U; // UndCC_Violation - a is assigned 255
    int32_t b = -a;  // UndCC_Violation - b is assigned -255
    uint32_t c = 1U;
    int64_t d = -c; // UndCC_Violation - d is assigned MAX_UINT
}

void g(uint32_t x, uint16_t y)
{
    f(-x); // UndCC_Violation - calls f( uint32_t a )
    f(-y); // UndCC_Violation - calls f( int32_t a )
}
