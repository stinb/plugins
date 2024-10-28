#include <cstdint>

int main(void)
{
    // 0xABCDEF should not be included because comment
    std::int16_t a = 0x0f0f;  // UndCC_Violation
    std::int16_t b = 0x0f0F;  // UndCC_Violation
    std::int16_t c = 0x0F0F; // UndCC_Valid
    std::int16_t d = 0xFFFF; // UndCC_Valid
    std::int16_t e = 0xaF0F;  // UndCC_Violation
    std::int16_t f = 0xaaaaaa;  // UndCC_Violation
    std::int16_t g = 0x0000; // UndCC_Valid
    std::int16_t h = 0x1fe2;  // UndCC_Violation


    return 0;
}