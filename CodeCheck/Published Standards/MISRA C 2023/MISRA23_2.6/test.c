#include <stdint.h>

void unused_label(void)
{
    int16_t x = 6;
label1: /* UndCC_Violation */
    use_int16(x);
}
