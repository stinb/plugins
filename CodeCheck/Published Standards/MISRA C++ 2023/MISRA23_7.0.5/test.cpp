#include <cstdint>

int32_t get_signed_value();
uint32_t get_unsigned_value();

void test_rule_7_0_5()
{
    uint8_t u8a = 1, u8b = 2;
    uint16_t u16a = 10;
    uint32_t u32a = 100;
    int32_t s32 = 20;
    float f32 = 1.0f;
    bool b = true;

    // Integral promotion (small unsigned → signed int)
    auto r1 = u8a + u8b;              // UndCC_Violation - both u8 → signed int
    auto r2 = u8a += u8b;             // UndCC_Violation - same as u8a + u8b
    auto r3 = ~u8a;                   // UndCC_Violation - u8a → signed int
    auto r4 = u8a << 2;               // UndCC_Violation - u8a → signed int
    auto r5 = -u8a;                   // UndCC_Violation - u8a → signed int (unary minus)
    auto r6 = +u8a;                   // UndCC_Violation - u8a → signed int (unary plus)

    // UAC (signed ↔ unsigned)
    auto r7 = s32 > u32a;             // UndCC_Violation - s32 → unsigned int
    auto r8 = s32 + u32a;             // UndCC_Violation - s32 → unsigned int

    // UAC (integral → floating)
    auto r9 = f32 += u32a;            // UndCC_Violation - u32a → float

    // Ternary operator
    auto r10 = b ? u8a : u16a;        // UndCC_Violation - both → signed int

    // EXCEPTION #1 (non-negative constant signed → unsigned)
    auto c1 = u32a - 1;               // UndCC_Valid - literal 1 is non-negative constant
    auto c2 = u32a > 10;              // UndCC_Valid - literal 10 is non-negative constant

    // EXCEPTION #2 (constant integral → floating)
    auto c4 = f32 += 1;               // UndCC_Valid - literal 1 is integral constant
    auto c5 = f32 * 2;                // UndCC_Valid - literal 2 is integral constant
    auto c6 = f32 + 0x10;             // UndCC_Valid - hex literal is integral constant

    // RULE DOES NOT APPLY
    auto n1 = u8a++;                  // UndCC_Valid - increment excluded
    auto n2 = ++u8a;                  // UndCC_Valid - increment excluded
    auto n3 = u8a--;                  // UndCC_Valid - decrement excluded
    auto n4 = --u8a;                  // UndCC_Valid - decrement excluded

    uint8_t arr[10];
    auto n5 = arr[u8a];               // UndCC_Valid - array indexing excluded

    // Expression evaluated before cast
    auto r33 = static_cast<uint16_t>(u8a + u8b);  // UndCC_Violation - u8a+u8b evaluated first

    // Cast before operation
    auto c7 = static_cast<uint32_t>(u8a) + u8b;  // UndCC_Valid - u8b: unsigned→unsigned

    // Same signedness
    auto c9 = u32a + u32a;            // UndCC_Valid - both unsigned, same size
    auto c10 = s32 + s32;             // UndCC_Valid - both signed, same size

    // Larger unsigned promotes unsigned
    auto c11 = u32a * u16a;           // UndCC_Valid - u16a promotes to int, then UAC → uint32_t

    // More complex cases
    auto r11 = u8a & u8b;             // UndCC_Violation - bitwise AND, both → signed int
    auto r12 = u8a | u8b;             // UndCC_Violation - bitwise OR, both → signed int
    auto r13 = u8a ^ u8b;             // UndCC_Violation - bitwise XOR, both → signed int
    auto r14 = u8a * u8b;             // UndCC_Violation - multiplication, both → signed int
    auto r15 = u8a / u8b;             // UndCC_Violation - division, both → signed int
    auto r16 = u8a % u8b;             // UndCC_Violation - modulo, both → signed int
    auto r17 = u8a == u8b;            // UndCC_Violation - equality, both → signed int
    auto r18 = u8a != u8b;            // UndCC_Violation - inequality, both → signed int
    auto r19 = u8a <= u8b;            // UndCC_Violation - less-equal, both → signed int
    auto r20 = u8a >= u8b;            // UndCC_Violation - greater-equal, both → signed int

    // Shift operators (right operand too, but less common)
    auto r21 = u16a << 3;             // UndCC_Violation - u16a → signed int
    auto r22 = u16a >> 2;             // UndCC_Violation - u16a → signed int
    auto r23 = u8a <<= 1;             // UndCC_Violation - u8a → signed int
    auto r24 = u8a >>= 1;             // UndCC_Violation - u8a → signed int

    // Same size, no promotion issues
    auto c12 = u32a << 2;             // UndCC_Valid - u32a doesn't promote
    auto c13 = s32 << 3;              // UndCC_Valid - s32 doesn't promote

    // Compound assignments
    auto r25 = u8a &= u8b;            // UndCC_Violation - both → signed int
    auto r26 = u8a |= u8b;            // UndCC_Violation - both → signed int
    auto r27 = u8a ^= u8b;            // UndCC_Violation - both → signed int
    auto r28 = u8a *= u8b;            // UndCC_Violation - both → signed int
    auto r29 = u8a /= u8b;            // UndCC_Violation - both → signed int
    auto r30 = u8a %= u8b;            // UndCC_Violation - both → signed int

    // Floating point conversions
    double d64 = 1.0;
    auto r31 = d64 + u32a;            // UndCC_Violation - u32a → double
    auto r32 = d64 += s32;            // UndCC_Violation - s32 → double

    // EXCEPTION - Constants to floating
    auto c14 = d64 + 100;             // UndCC_Valid - literal 100 is integral constant
    auto c15 = f32 - 50;              // UndCC_Valid - literal 50 is integral constant

    // Test unary minus in various contexts
    auto r34 = (-u8a);                // UndCC_Violation - unary minus after (
    auto r35 = u32a + (-u8b);         // UndCC_Violation - unary minus in expression
    auto r36 = u32a * -u16a;          // UndCC_Violation - unary minus after operator
    int32_t s8 = -5;                  // UndCC_Valid - literal doesn't promote
    auto r37 = -s32;                  // UndCC_Valid - signed int, no signedness change
    auto r38 = +u16a;                 // UndCC_Violation - unary plus on small unsigned

    // Ensure binary minus still works
    auto c16 = s32 - s32;             // UndCC_Valid - binary minus, no conversion
    auto c17 = u32a - u32a;           // UndCC_Valid - binary minus, no conversion

    // Test assignment in parentheses (Apache APR pattern - false positive fix)
    int32_t rv;
    int32_t result = 0;
    if ((rv = result) != 0) {         // UndCC_Valid - both signed int, no conversion
        // This pattern should not flag - assignment in parens compared to literal
    }

    uint32_t rv2;
    if ((rv2 = u32a) != 1) {          // UndCC_Valid - both unsigned, literal is non-negative
        // Unsigned var assigned and compared to non-negative constant
    }

    // Test function call return values (Apache APR pattern - false positive fix)
    if (get_signed_value() != 0) {    // UndCC_Valid - both signed int, no conversion
        // Function returning signed int compared to literal 0
    }

    if (get_unsigned_value() != 1) {  // UndCC_Valid - unsigned compared to non-negative constant
        // Function returning unsigned compared to literal (exception #1)
    }

    // Test parenthesized arithmetic expressions (Apache APR pattern - false positive fix)
    int32_t min_spare = 10;
    int32_t threads_per = 5;
    int32_t num_buckets = 3;
    if (min_spare < threads_per * (num_buckets - 1) + num_buckets) {  // UndCC_Valid - all signed int
        min_spare = threads_per * (num_buckets - 1) + num_buckets;    // UndCC_Valid - all signed int
    }

    auto r40 = s32 * (s32 + s32);     // UndCC_Valid - all signed, no conversion
    auto r41 = s32 + (s32 - s32);     // UndCC_Valid - all signed, no conversion
}

int32_t get_signed_value() { return 0; }
uint32_t get_unsigned_value() { return 0; }
