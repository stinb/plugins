// Reproducer for crash #789
#include <string.h>
#include <stdint.h>

struct S {
    uint8_t  buf[16];
    uint16_t ibuf[8];
};

/* A: ordinary memcpy with local-array lvalues */
void f_local(void) {
    char a[16];
    char b[16];
    memcpy(a, b, 16);            // UndCC_Valid
    memcmp(a, b, 16);            // UndCC_Valid
    memmove(a, b, 16);           // UndCC_Valid
}

/* B: ordinary memcpy on struct member arrays */
void f_member(struct S *s) {
    memcpy(s->buf, s->buf, 16);   // UndCC_Valid
    memcmp(s->buf, s->ibuf, 16);  // UndCC_Violation
}

/* C: FORTIFY builtin with local-array lvalues */
void f_fortify(void) {
    char a[16];
    char b[16];
    __builtin___memcpy_chk(a, b, 16, __builtin_object_size(a, 0));      // UndCC_Valid
    __builtin___memmove_chk(a, b, 16, __builtin_object_size(a, 0));     // UndCC_Valid
}
