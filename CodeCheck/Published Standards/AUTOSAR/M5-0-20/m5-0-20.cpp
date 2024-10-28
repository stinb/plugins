#include <cstdint>

typedef int INT;

void foo()
{
    uint8_t mask = ~(0x10);
    uint16_t value;
    INT new_value;
    value ^= mask; // UndCC_Violation
    new_value |= mask;  // UndCC_Violation
}
