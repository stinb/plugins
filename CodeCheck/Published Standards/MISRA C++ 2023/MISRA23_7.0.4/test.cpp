#include <cstdint>

void foo()
{
    uint8_t u8 = 5;
    uint16_t u16 = 10;
    uint32_t u32 = 100;
    uint32_t u32a = 50;
    uint32_t u32b = 75;
    int32_t s32 = 20;
    int32_t s32a = 30;
    int32_t s32b = 40;

    // Shift operators - left operand
    auto r1 = 1u << u8;     // UndCC_Valid
    auto r2 = 1u << 31;     // UndCC_Valid
    auto r3 = uint8_t(1) << 2;    // UndCC_Valid - but violates other rules
    auto r4 = u8 << 2;      // UndCC_Valid - but violates other rules
    auto r5 = s32 << 2;     // UndCC_Violation - left operand is signed
    auto r6 = 1 << u8;      // UndCC_Violation - left operand is signed
    auto r7 = (u8 + u16) << 2; // UndCC_Violation - result of + is signed
    auto r8 = static_cast<uint16_t>(u8 + u16) << 2; // UndCC_Valid
    auto r9 = 1LL << 31;    // UndCC_Valid by exception
    auto r10 = 1 << 30;     // UndCC_Valid by exception
    auto r11 = 2 << 30;     // UndCC_Violation - exception does not apply as set bit is shifted too far

    // Binary bitwise operators
    auto r12 = u32a | u32b; // UndCC_Valid
    auto r13 = s32a | s32b; // UndCC_Violation - signed operands
    auto r14 = u32a & u32b; // UndCC_Valid
    auto r15 = s32a & s32b; // UndCC_Violation - signed operands
    auto r16 = u32a ^ u32b; // UndCC_Valid
    auto r17 = s32a ^ s32b; // UndCC_Violation - signed operands

    // Bit complement operator
    auto r18 = ~u32;        // UndCC_Valid
    auto r19 = ~u8;         // UndCC_Valid - but violates other rules
    auto r20 = ~s32;        // UndCC_Violation - signed operand

    // Compound assignment forms
    u32a |= u32b;           // UndCC_Valid
    s32a |= s32b;           // UndCC_Violation - signed operands
    u32a &= u32b;           // UndCC_Valid
    s32a &= s32b;           // UndCC_Violation - signed operands
    u32a ^= u32b;           // UndCC_Valid
    s32a ^= s32b;           // UndCC_Violation - signed operands
    u32 <<= 2;              // UndCC_Valid
    s32 <<= 2;              // UndCC_Violation - left operand is signed
    u32 >>= 2;              // UndCC_Valid
    s32 >>= 2;              // UndCC_Violation - left operand is signed
}

void some_func(int a, int32_t *b) {}

// Address-of operator (should not trigger)
void test_address_of()
{
    int x = 5;
    int *ptr = &x;                    // UndCC_Valid - address-of
    int32_t val = 10;
    some_func(0, &val);               // UndCC_Valid - address-of in function call
}

// Reference declarations (should not trigger)
void test_reference()
{
    int32_t a = 5;
    int32_t &ref = a;                 // UndCC_Valid - reference declaration
}
