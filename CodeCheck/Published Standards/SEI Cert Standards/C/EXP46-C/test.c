#include <stdint.h>

#if __has_include(<unistd.h>)

#include <unistd.h>
int main() {

  if (!(getuid() & geteuid() == 0)) { // UNDCC_Violation(lin,mac)
    /* ... */
  }

  if (!(getuid() && geteuid() == 0)) { // UndCC_Valid
    /* ... */
  }
}

#endif // __has_include(<unistd.h>)

// Test case from bug report
void test_false_positive() {
    uint8_t c_d = 0;
    uint16_t g_sci = 0x1245;

    if ( (c_d == 1) && ((g_sci & 0x0002U) != 0) ) { ; } // UndCC_Valid
}

// Missing parentheses
void test_real_violation() {
    uint8_t a = 5;
    uint8_t b = 10;

    if (a & b == 5) { ; }  // UndCC_Violation
}

// Properly parenthesized bitwise operation
void test_compliant() {
    uint16_t flags = 0x1234;

    if ((flags & 0x0001U) != 0) { ; } // UndCC_Valid
}

#define ID0 0x40U
#define ID1 0x20U

uint16_t read_register(int addr, int size);

void test_macro_with_nested_bitwise() {
    if ( (read_register(0x1234,1) & ( ID0 | ID1 ) ) != ( ID0 | ID1 ) ) { ; } // UndCC_Valid
}
