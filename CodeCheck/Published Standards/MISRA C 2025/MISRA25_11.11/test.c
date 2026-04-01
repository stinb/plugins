#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// Compliant - explicit comparisons
void test_compliant(uint32_t *ptr, int *p2) {
    if (ptr != NULL) { }                // UndCC_Valid
    if (ptr == NULL) { }                // UndCC_Valid
    if (ptr != 0) { }                   // UndCC_Valid - explicit test for zero
    while (ptr != NULL) { }             // UndCC_Valid
    do { } while (ptr != NULL);         // UndCC_Valid
    for (; ptr != NULL; ) { }           // UndCC_Valid
    (_Bool)(ptr != NULL);               // UndCC_Valid
    (bool)(ptr != NULL);                // UndCC_Valid
    ptr != NULL ? 1 : 0;               // UndCC_Valid
    p2 == NULL ? 1 : 0;                // UndCC_Valid
    if (ptr != NULL && *ptr == 1) { }  // UndCC_Valid
    if (ptr != NULL || p2 != NULL) { } // UndCC_Valid
    if ((ptr) != NULL) { }             // UndCC_Valid - parens around pointer
}

// Non-compliant - implicit comparisons
void test_noncompliant(uint32_t *ptr, int *p2) {
    if (ptr) { }                        // UndCC_Violation
    if (!ptr) { }                       // UndCC_Violation
    if (!(ptr)) { }                     // UndCC_Violation - ! through parens
    while (ptr) { }                     // UndCC_Violation
    do { } while (ptr);                 // UndCC_Violation
    for (; ptr; ) { }                   // UndCC_Violation
    if (ptr && *ptr == 1) { }          // UndCC_Violation
    if (p2 || *p2 == 0) { }           // UndCC_Violation
    if (ptr) { }                        // UndCC_Violation - left of ||
    if (p2) { }                         // UndCC_Violation - right of ||
    ptr ? 1 : 0;                       // UndCC_Violation
    (bool)(ptr);                        // UndCC_Violation
    (_Bool)(ptr);                       // UndCC_Violation
}

// Function pointers - same rules apply
void test_func_ptr(void) {
    void (*fp)(void) = (void (*)(void))0;
    if (fp != NULL) { }                 // UndCC_Valid
    if (fp) { }                         // UndCC_Violation
    !fp;                                // UndCC_Violation
    fp ? 1 : 0;                         // UndCC_Violation
}

typedef struct { uint32_t x; } obj_t;

// Non-pointer usage - should not flag
void test_no_flag(uint32_t *ptr, obj_t *sp) {
    uint32_t x = *ptr;                  // UndCC_Valid - dereference
    sp->x;                              // UndCC_Valid - member access
    uint32_t *q = ptr;                  // UndCC_Valid - assignment
    ptr++;                              // UndCC_Valid - increment
    int val = 0;
    if (val) { }                        // UndCC_Valid - not a pointer
    if (!val) { }                       // UndCC_Valid - not a pointer
}
