/* Test for MISRA C 2012/2023/2025 Rule 14.3 - Controlling expressions
 * shall not be invariant.
 */

typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;
typedef signed   char  int8_t;
typedef signed   short int16_t;
typedef int            int32_t;
typedef int            bool_t;

#define true  1
#define false 0

extern void use(int);

/* ===== Literal-only controlling expressions ===== */

void literal_conditions(void)
{
    if (5)                            /* UndCC_Violation - literal 5 */
    {
        use(1);
    }

    if (0)                            /* UndCC_Violation - literal 0 */
    {
        use(1);
    }

    if (1)                            /* UndCC_Violation - literal 1 */
    {
        use(1);
    }

    while (42)                        /* UndCC_Violation - literal 42 */
    {
        break;
    }

    switch (3)                        /* UndCC_Violation - literal switch */
    {
    case 3:
        break;
    default:
        break;
    }

    int x = (0) ? 1 : 2;              /* UndCC_Violation - ternary literal cond */
    use(x);
}

/* ===== Literal-vs-literal comparisons ===== */

void literal_comparisons(void)
{
    if (2 > 3)                        /* UndCC_Violation - always false */
    {
        use(1);
    }

    if (5 == 5)                       /* UndCC_Violation - always true */
    {
        use(1);
    }

    do
    {
        use(1);
    } while (0u == 1u);               /* UndCC_Violation - always false (not exception 2) */
}

/* ===== const variable invariants ===== */

void const_variable_invariants(void)
{
    const uint8_t numcyl = 4u;

    if (numcyl == 4u)                 /* UndCC_Violation - folds to true */
    {
        use(1);
    }

    if (numcyl < 10)                  /* UndCC_Violation - folds to true */
    {
        use(1);
    }
}

/* ===== static const / file-scope const ===== */

static const int32_t s_threshold = 10;
const int32_t g_limit = 100;

void static_and_global_const(int32_t a)
{
    if (s_threshold == 10)            /* UndCC_Violation - static const folds */
    {
        use(1);
    }

    if (g_limit > 0)                  /* UndCC_Violation - file-scope const folds */
    {
        use(1);
    }

    /* Compared against a runtime value -> not invariant. */
    if (a < s_threshold)              /* UndCC_Valid - depends on a */
    {
        use(1);
    }
}

/* ===== enum constants ===== */

enum colors { RED = 0, GREEN = 1, BLUE = 2 };

void enum_constants(enum colors c)
{
    if (RED == 0)                     /* UndCC_Violation - enumerator folds */
    {
        use(1);
    }

    if (GREEN)                        /* UndCC_Violation - enumerator folds */
    {
        use(1);
    }

    switch (BLUE)                     /* UndCC_Violation - enumerator folds */
    {
    case BLUE:
        break;
    default:
        break;
    }

    /* Compared against a runtime value -> not invariant. */
    switch (c)                        /* UndCC_Valid - depends on c */
    {
    case RED:
        break;
    default:
        break;
    }
}

/* ===== const volatile - compliant ===== */

void calibration_value(void)
{
    const volatile uint8_t numcyl_cal = 4u;

    /* volatile inhibits constant folding -> compliant */
    if (numcyl_cal == 4u)             /* UndCC_Valid - volatile not folded */
    {
        use(1);
    }
}

/* ===== Unsigned compared to zero ===== */

void unsigned_vs_zero(uint16_t u)
{
    if (u < 0u)                       /* UndCC_Violation - unsigned never < 0 */
    {
        use(1);
    }

    if (u >= 0u)                      /* UndCC_Violation - unsigned always >= 0 */
    {
        use(1);
    }

    if (0u > u)                       /* UndCC_Violation - mirror form */
    {
        use(1);
    }

    if (0u <= u)                      /* UndCC_Violation - mirror form */
    {
        use(1);
    }

    /* Ternary with unsigned-vs-zero first operand. */
    int s = (u < 0u) ? 0 : 1;         /* UndCC_Violation - ternary cond invariant */
    use(s);
}

/* ===== Allowed exception forms ===== */

void exception_forms(void)
{
    for (;;)                          /* UndCC_Valid - exception 1a */
    {
        break;
    }

    while (true)                      /* UndCC_Valid - exception 1b */
    {
        break;
    }

    while (1)                         /* UndCC_Valid - exception 1b (literal 1) */
    {
        break;
    }

    do                                /* UndCC_Valid - exception 1c */
    {
        use(1);
    } while (true);

    do                                /* UndCC_Valid - exception 1c (literal 1) */
    {
        use(1);
    } while (1);

    do                                /* UndCC_Valid - exception 2 */
    {
        use(1);
    } while (false);

    do                                /* UndCC_Valid - exception 2 (literal 0) */
    {
        use(1);
    } while (0);

    /* for (; 1; ) is also treated as the idiomatic infinite-loop form. */
    for (int i = 0; 1; ++i)           /* UndCC_Valid - infinite for-loop */
    {
        (void)i;
        break;
    }
}

/* ===== Non-invariant cases - must NOT be flagged ===== */

void valid_conditions(int32_t a, int32_t b, uint16_t u)
{
    if (a > b)                        /* UndCC_Valid - runtime comparison */
    {
        use(1);
    }

    while (a < 10)                    /* UndCC_Valid - depends on parameter */
    {
        ++a;
    }

    for (int i = 0; i < a; ++i)       /* UndCC_Valid - bound is a parameter */
    {
        use(i);
    }

    do
    {
        ++a;
    } while (a < b);                  /* UndCC_Valid - runtime comparison */

    switch (a)                        /* UndCC_Valid - parameter */
    {
    case 1:
        break;
    default:
        break;
    }

    int x = (a > b) ? 1 : 2;          /* UndCC_Valid - runtime ternary */
    use(x);

    /* unsigned compared to non-zero literal is not in our special case. */
    if (u < 5u)                       /* UndCC_Valid - depends on u */
    {
        use(1);
    }

    /* Equality/inequality against zero is NOT in the unsigned-vs-zero special
     * case (only <, >=, >, <= are). The value depends on u, so compliant. */
    if (u != 0u)                      /* UndCC_Valid - depends on u */
    {
        use(1);
    }
}

/* ===== Macro-expanded constants - intentionally not flagged ===== */

/* The check intentionally ignores controlling expressions originating from
 * macro expansions. `#define DEBUG 1` ... `if (DEBUG)` is a common idiom for
 * a configurable build-time flag, and flagging it would be noisy. Suppress on
 * the use-site with UndCC_SuppressLine if a strict reading is needed. */

#define DEBUG       1
#define BUILD_FLAGS 0
#define MAX_RETRIES 5

void macro_constants(void)
{
    if (DEBUG)                        /* UndCC_Valid - macro expansion ignored */
    {
        use(1);
    }

    if (BUILD_FLAGS == 0)             /* UndCC_Valid - macro expansion ignored */
    {
        use(1);
    }

    if (MAX_RETRIES > 0)              /* UndCC_Valid - macro expansion ignored */
    {
        use(1);
    }
}

/* ===== Nested controlling expressions ===== */

void nested(int32_t a)
{
    if (a > 0)                        /* UndCC_Valid */
    {
        if (1)                        /* UndCC_Violation - nested literal */
        {
            use(1);
        }
    }

    while (a < 100)                   /* UndCC_Valid */
    {
        if (2 == 2)                   /* UndCC_Violation - nested const-fold */
        {
            break;
        }
    }
}
