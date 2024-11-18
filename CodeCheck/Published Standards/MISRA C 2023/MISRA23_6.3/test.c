#include <stdint.h>

/* Compliant - if the user wants to type-pun, the bits of 'big' which will
 ... be overlaid are clearly identified */
union U1 // UndCC_Valid
{
    uint8_t small;
    uint32_t big;
};

/* Non-compliant - it is unclear which, if any, bits of 'big' are overlaid by
 ... 'small' in this type */
union U2
{
    uint32_t small : 8; // UndCC_Violation
    uint32_t big;
};

/* Non-compliant - it is unclear if any bits of 'big' are overlaid by
 ... 'small' in this type */
union U3
{
    uint32_t small : 8; // UndCC_Violation
    uint32_t big : 24;
};

/* Compliant - a sub-object can be a bit-field */
union U4
{
    struct
    {
        uint8_t a : 4; // UndCC_Valid
        uint8_t b : 4; // UndCC_Valid
        uint8_t c : 4; // UndCC_Valid
        uint8_t d : 4; // UndCC_Valid
    } q;
    uint16_t r;
};
