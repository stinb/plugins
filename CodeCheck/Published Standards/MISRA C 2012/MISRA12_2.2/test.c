/* Test for MISRA C 2012/2023/2025 Rule 2.2 - There shall be no dead code. */

typedef unsigned short uint16_t;
typedef int            int32_t;
typedef int            bool_t;
typedef int            Alias;

extern volatile uint16_t v;
extern char *p;
extern int  global_x;
extern int *global_pi;

extern void use(int);
extern int  side_effect_fn(void);

void f1(void)
{
    uint16_t x;

    /* MISRA examples - compliant. */
    (void) v;                      /* UndCC_Valid - explicit discard */
    (*p)++;                        /* UndCC_Valid - increments *p */

    /* MISRA examples - non-compliant. */
    (int32_t) v;                   /* UndCC_Violation - cast result unused */
    v >> 3;                        /* UndCC_Violation - shift result unused */
    *p++;                          /* UndCC_Violation - deref result unused */

    /* x = 3 (Note 3, "x not subsequently read") is intentionally NOT
     * detected here; it is reported by CPP_V017 / AUTOSAR A0-1-1. */
    x = 3;                         /* UndCC_Valid - covered by CPP_V017 */
}

void f2(void)
{
    int a = 1;
    int b = 2;
    int c;

    /* Bare expression statements - all dead. */
    a + b;                         /* UndCC_Violation - sum unused */
    a == b;                        /* UndCC_Violation - typo for '='? */
    a != b;                        /* UndCC_Violation - comparison unused */
    a < b;                         /* UndCC_Violation - comparison unused */
    a <= b;                        /* UndCC_Violation */
    a > b;                         /* UndCC_Violation */
    a >= b;                        /* UndCC_Violation */
    a & b;                         /* UndCC_Violation */
    a | b;                         /* UndCC_Violation */
    a ^ b;                         /* UndCC_Violation */
    a << 1;                        /* UndCC_Violation */
    a >> 1;                        /* UndCC_Violation */
    !a;                            /* UndCC_Violation */
    ~a;                            /* UndCC_Violation */
    -a;                            /* UndCC_Violation */
    a;                             /* UndCC_Violation - bare identifier */

    /* Compliant: assignment or call. */
    c = a + b;                     /* UndCC_Valid - assigns c */
    c = a == b;                    /* UndCC_Valid */
    c += a;                        /* UndCC_Valid */
    c -= a;                        /* UndCC_Valid */
    c *= a;                        /* UndCC_Valid */
    c /= b;                        /* UndCC_Valid - real division */
    c %= b;                        /* UndCC_Valid */
    c <<= 1;                       /* UndCC_Valid */
    c >>= 1;                       /* UndCC_Valid */
    c &= a;                        /* UndCC_Valid */
    c |= a;                        /* UndCC_Valid */
    c ^= a;                        /* UndCC_Valid */
    a++;                           /* UndCC_Valid - has side effect */
    ++a;                           /* UndCC_Valid */
    a--;                           /* UndCC_Valid */
    --a;                           /* UndCC_Valid */
    use(a);                        /* UndCC_Valid - function call */
    side_effect_fn();              /* UndCC_Valid - function call */
}

struct S { int field; };

void f3(struct S *s, int *arr, int i)
{
    /* Bare member / subscript / deref. */
    s->field;                      /* UndCC_Violation - member access unused */
    (*s).field;                    /* UndCC_Violation - member access unused */
    arr[i];                        /* UndCC_Violation - subscript unused */
    *arr;                          /* UndCC_Violation - deref unused */

    /* Compliant: write through. */
    s->field = 0;                  /* UndCC_Valid */
    arr[i] = 0;                    /* UndCC_Valid */
    *arr = 0;                      /* UndCC_Valid */
    use(s->field);                 /* UndCC_Valid */
    use(arr[i]);                   /* UndCC_Valid */
    use(*arr);                     /* UndCC_Valid */

    /* The *<expr>++/-- pattern: deref result unused. */
    *p++;                          /* UndCC_Violation */
    *p--;                          /* UndCC_Violation */

    /* Compliant: parenthesized deref then increment. */
    (*p)++;                        /* UndCC_Valid */
    (*p)--;                        /* UndCC_Valid */
}

void f4(void)
{
    int x = 0;
    int y = 0;

    /* Declarations are not expression statements. */
    int z;                         /* UndCC_Valid - declaration */
    int  w = 1;                    /* UndCC_Valid - declaration with init */
    static int s = 5;              /* UndCC_Valid */
    const int k = 7;               /* UndCC_Valid */
    struct S local_s;              /* UndCC_Valid */

    /* Control flow is not analyzed for dead code. */
    if (x == y) {                  /* UndCC_Valid - controlling expr */
        use(x);
    }
    while (x < 10) {               /* UndCC_Valid */
        x++;
    }
    for (x = 0; x < 10; x++) {     /* UndCC_Valid */
        use(x);
    }
    do {
        x++;
    } while (x < 5);               /* UndCC_Valid */
    switch (x) {                   /* UndCC_Valid */
        case 1:
            use(x);
            break;
        default:
            break;
    }
    return;                        /* UndCC_Valid */
}

void f5(void)
{
    /* __asm is a language extension and not dead code. */
    __asm("NOP");                  /* UndCC_Valid - inline asm */
}

/* Function-like statements at top level (struct decl in function) shouldn't
 * be analyzed as dead-code candidates. */
void f6(void)
{
    struct Local { int x; int y; };  /* UndCC_Valid - local struct */
    struct Local lc = { 1, 2 };      /* UndCC_Valid - declaration with init */
    use(lc.x);                       /* UndCC_Valid */

    /* Bare cast that's not '(void)'. */
    (int) v;                       /* UndCC_Violation - cast to non-void */
    (char) global_x;               /* UndCC_Violation */
    (Alias) v;                     /* UndCC_Violation - cast to typedef */
}

/* Compliant trailing example: empty block, empty statements. */
void f7(void)
{
    ;                              /* UndCC_Valid - empty statement */
    ;;                             /* UndCC_Valid - two empty statements */
    {                              /* UndCC_Valid - bare block */
        use(1);
    }
}

/* Pointer-to-function call should not be flagged. */
void f8(void (*fp)(int))
{
    (*fp)(42);                     /* UndCC_Valid - call via deref */
    fp(42);                        /* UndCC_Valid - call directly */
}

/* Comma operator: result of last sub-expression is unused. Only the
 * first sub-expression is examined for side effects in this conservative
 * subset; comma expressions whose right-hand side is a bare value are
 * detected via the overall statement having no side-effect tokens. */
void f9(void)
{
    int a = 0;
    int b = 0;
    a, b;                          /* UndCC_Violation - both sides dead */
    (a = 1), b;                    /* UndCC_Valid - left has side effect (assignment) */
    a++, b;                        /* UndCC_Valid - left has ++ */
}

/* Multi-line `#define` inside a function body (libgit2 idiom). All lines
 * of the macro definition - including continuation lines starting with
 * `else`, `if`, identifiers, etc. - must be ignored. */
void f10(int *out, int n)
{
#define ASSIGN_IF(ptr, val)   \
    if ((val) == 0)           \
        ptr = 0;              \
    else {                    \
        *(ptr) = (val);       \
        ptr++;                \
    }

    ASSIGN_IF(out, n);             /* UndCC_Valid - macro call */
}

/* MISRA example: the call to g is non-compliant because g has no
 * operations and the call could be removed without affecting program
 * behaviour. Detecting this requires whole-program analysis, so this
 * check intentionally does not report it. */
void g(void)
{
    /* Compliant - there are no operations in this function */
}

void h(void)
{
    g();                           /* UndCC_Valid - empty-fn call, not detected (needs whole-program analysis) */
}
