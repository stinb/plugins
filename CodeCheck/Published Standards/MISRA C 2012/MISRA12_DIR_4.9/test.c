#include <stdint.h>
#include <stdio.h>

// Object-like macros are not function-like; not subject to Directive 4.9.
#define PI 3.14
#define MAX 100

// Function-like macros that are interchangeable with a function. The
// definitions themselves are not the violation per the standard — the
// violation is at a use site where a function call is permitted.
#define DIV2(X) ( ( X ) / 2 )
#define SQUARE(X) ( ( X ) * ( X ) )
#define MIN(A, B) ( ( (A) < (B) ) ? (A) : (B) )

// Has a C operator as an argument — no function is interchangeable.
#define EVAL_BINOP(OP, L, R) ( ( L ) OP ( R ) )

// Uses # (stringification) — cannot be a function.
#define STRINGIFY(x) #x

// Uses ## (token pasting) — cannot be a function.
#define CONCAT(a, b) a ## b

// Uses predefined identifiers that must expand at the call site.
#define LOG_HERE(msg) fprintf(stderr, "%s:%d: %s\n", __FILE__, __LINE__, msg)
#define TRACE() printf("in %s\n", __func__)
#define TRACE_GCC() printf("in %s\n", __FUNCTION__)
#define TRACE_PRETTY() printf("in %s\n", __PRETTY_FUNCTION__)

// File-scope object with static storage — a function call is not
// permitted here, so macro use is compliant.
static uint16_t g_half = DIV2(20);                      // UndCC_Valid

void f(void)
{
    // Static-storage initializer — function call not permitted. Compliant.
    static uint16_t sx = DIV2(10);                      // UndCC_Valid
    static uint16_t ss = SQUARE(3);                     // UndCC_Valid

    // Automatic variable — function call permitted. Non-compliant.
    uint16_t y = DIV2(10);                              // UndCC_Violation
    uint16_t q = SQUARE(4);                             // UndCC_Violation
    uint16_t m = MIN(y, q);                             // UndCC_Violation

    // Operator argument — no function is interchangeable. Compliant.
    uint32_t z = EVAL_BINOP(+, 1, 2);                   // UndCC_Valid
    uint32_t w = EVAL_BINOP(*, 3, 4);                   // UndCC_Valid

    // # / ## — cannot be a function. Compliant.
    const char *s = STRINGIFY(hello);                   // UndCC_Valid
    int CONCAT(foo, bar) = 0;                           // UndCC_Valid

    // Predefined identifier macros — a function would capture the wrong
    // location. Compliant.
    LOG_HERE("oops");                                   // UndCC_Valid
    TRACE();                                            // UndCC_Valid
    TRACE_GCC();                                        // UndCC_Valid
    TRACE_PRETTY();                                     // UndCC_Valid

    // case label — constant expression required, function call not permitted.
    switch (y)
    {
        case DIV2(2):                                   // UndCC_Valid
            break;
        case SQUARE(3):                                 // UndCC_Valid
            break;
        default:
            break;
    }

    (void)sx; (void)ss; (void)y; (void)q; (void)m;
    (void)z; (void)w; (void)s; (void)foobar;
}
