/* Test for MISRA C 2012/2023/2025 Rule 13.2 - The value of an expression
 * and its persistent side effects shall be the same under all permitted
 * evaluation orders.
 *
 * Conservative subset: detects subrule 1 (object modified more than once)
 * and subrule 3 (object modified and read where read does not contribute
 * to the new value). Subrules 4-6 (volatile/atomic) are not implemented.
 */

typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;
typedef signed   char  int8_t;
typedef signed   short int16_t;
typedef int            int32_t;

extern void f(int, int);
extern int  g(void);
extern int  h(int);
extern void use(int);

/* ===== Subrule 1: modified more than once ===== */

void modified_twice(void)
{
    int i = 0;
    int j = 0;
    int x = 0;
    int a[10];
    int b[10];

    j = i++ + i++;                        /* UndCC_Violation - i modified twice */
    j = ++i + i++;                        /* UndCC_Violation - i modified twice */
    j = i-- - i++;                        /* UndCC_Violation - i modified twice */
    i = i++;                              /* UndCC_Violation - i modified twice */
    x = x = 0;                            /* UndCC_Violation - x modified twice (Note 2) */
    i = ++i;                              /* UndCC_Violation - i modified twice */

    a[i++] = b[i++];                      /* UndCC_Violation - i modified twice (MISRA standard example: COPY_ELEMENT(i++)) */

    use(i);
    use(j);
    use(x);
    use(a[0]);
    use(b[0]);
}

/* ===== Subrule 3: modified and read elsewhere ===== */

void modified_and_read(void)
{
    int a[10];
    int i = 0;
    int j = 0;

    a[i] = i++;                           /* UndCC_Violation - i read in a[i], modified by i++ */
    j = i + i++;                          /* UndCC_Violation - i read and modified */
    f(i, i++);                            /* UndCC_Violation - i read and modified across args */
    j = i++ * i;                          /* UndCC_Violation - i modified and read */

    use(j);
    use(a[0]);
}

/* ===== Compliant cases - reads contribute to the new value ===== */

void read_contributes(void)
{
    int x = 0;
    int y = 1;
    int a[10];

    x = x + 1;                            /* UndCC_Valid - read of x contributes */
    x = x * 2 + x;                        /* UndCC_Valid - all reads of x contribute */
    x += y;                               /* UndCC_Valid - compound assignment */
    x -= x;                               /* UndCC_Valid - read in RHS contributes */
    x *= x + 1;                           /* UndCC_Valid */
    x = (y > 0) ? x : -x;                 /* UndCC_Valid - reads in ternary branches contribute */

    a[0] = 1;                             /* UndCC_Valid */
    use(x);
}

/* ===== Compliant: isolated ++ / -- ===== */

void isolated_inc_dec(void)
{
    int x = 0;
    int *p = &x;
    int *q = &x;
    int a[10];
    int i = 0;

    x++;                                  /* UndCC_Valid */
    ++x;                                  /* UndCC_Valid */
    x--;                                  /* UndCC_Valid */
    --x;                                  /* UndCC_Valid */

    a[i++] = 0;                           /* UndCC_Valid - i modified once, no other access */
    use(a[i++]);                          /* UndCC_Valid */

    (*p)++;                               /* UndCC_Valid - p not modified, *p is not a VarDecl mod */
    *p++;                                 /* UndCC_Valid - p's read contributes to its own value computation */

    *p++ = *q++;                          /* UndCC_Valid - p and q each modified once, no other access */

    use(x);
    use(*p);
    use(*q);
}

/* ===== Sequence points break unsequenced regions ===== */

void sequence_points_split(int cond)
{
    int i = 0;
    int j = 0;

    /* Comma operator is a sequence point */
    j = (i++, i);                         /* UndCC_Valid - separated by comma */

    /* Logical && and || are sequence points */
    if ((i++) && (i > 0)) {               /* UndCC_Valid - && is a sequence point */
        use(i);
    }
    if ((i++) || (i == 0)) {              /* UndCC_Valid - || is a sequence point */
        use(i);
    }

    /* Ternary ?: introduces sequence points */
    j = cond ? i++ : i;                   /* UndCC_Valid - cond and branches separated */

    use(j);
}

/* ===== Nested assignments ===== */

void nested_assignments(void)
{
    int x = 0;
    int y = 0;
    int z = 5;

    x = y = z;                            /* UndCC_Valid - different vars, one mod each */
    x = (y = z) + 1;                      /* UndCC_Valid */
    y = (x = z);                          /* UndCC_Valid */
}

/* ===== Function calls and side effects ===== */

void function_calls(void)
{
    int i = 0;
    int j = 0;

    j = g() + h(i);                       /* UndCC_Valid - no var modified twice */
    j = h(i++) + i;                       /* UndCC_Violation - i modified by h() arg and read */
    f(i++, j);                            /* UndCC_Valid - only i modified, no other access */
    f(i, j++);                            /* UndCC_Valid - i read once, j modified once */

    use(i);
    use(j);
}

/* ===== Conditions and initializers ===== */

void conditions_and_inits(void)
{
    int i = 0;

    if (i++ > i) {                        /* UndCC_Violation - i modified and read */
        use(i);
    }

    while (i++ < i) {                     /* UndCC_Violation - i modified and read */
        break;
    }

    /* for loop init / cond / inc are independent full expressions */
    for (int k = i++; k < i; ++k) {       /* UndCC_Valid - init/cond/inc separate */
        use(k);
        break;
    }

    int n = i++ + i;                      /* UndCC_Violation - initializer is one full expression */
    use(n);
}

/* ===== sizeof is unevaluated ===== */

void sizeof_unevaluated(void)
{
    int i = 0;
    int j = 0;

    j = sizeof(i++) + i;                  /* UndCC_Valid - i++ in sizeof not evaluated */
    j = sizeof(i++);                      /* UndCC_Valid */

    use(j);
}

/* ===== Multiple reads of the same variable - no modification ===== */

void multiple_reads(void)
{
    int i = 0;
    int j = 0;

    f(i, i);                              /* UndCC_Valid - i read twice, no mod */
    j = i + i;                            /* UndCC_Valid - i read twice */
    j = i * i + i;                        /* UndCC_Valid */

    use(j);
}

/* ===== Adjacent statements are independent full expressions ===== */

void adjacent_statements(void)
{
    int i = 0;
    int j = 0;

    i++; i++;                             /* UndCC_Valid - each is its own full expression */
    j = i++; j = i++;                     /* UndCC_Valid */

    use(i);
    use(j);
}

/* ===== Comma operator separates same-variable modifications ===== */

void comma_separates_mods(void)
{
    int i = 0;
    int j = 0;

    (void)(i++, i++);                     /* UndCC_Valid - comma is a sequence point */
    j = (i++, i);                         /* UndCC_Valid - covered in sequence_points_split also */
    (void)(i = 0, i = 1);                 /* UndCC_Valid - two assignments separated by comma */

    use(i);
    use(j);
}

/* ===== Multi-declarator initializers are independent full expressions ===== */

void multi_declarator_inits(void)
{
    int i = 0;

    /* Each declarator's initializer is a separate full expression. */
    int x = i++, y = i++;                 /* UndCC_Valid - two separate full expressions */
    int p = i, q = i;                     /* UndCC_Valid */

    use(x);
    use(y);
    use(p);
    use(q);
}

/* ===== Unbraced control-flow bodies are still full expressions ===== */

void unbraced_bodies(int cond)
{
    int i = 0;
    int j = 0;
    int a[10];

    if (cond) i = i++;                    /* UndCC_Violation - i modified twice */
    if (cond) a[i] = i++;                 /* UndCC_Violation - i modified and read */
    else      j = i++ + i++;              /* UndCC_Violation - i modified twice */

    while (cond) a[i] = i++;              /* UndCC_Violation */

    do j = i + i++; while (0);            /* UndCC_Violation - i modified and read */

    for (int k = 0; k < 1; ++k) j = i++ * i;  /* UndCC_Violation - body */

    /* Compliant unbraced bodies should still be quiet */
    if (cond) i++;                        /* UndCC_Valid */
    while (cond) i = i + 1;               /* UndCC_Valid */
    for (int k = 0; k < 1; ++k) i = i + 1;    /* UndCC_Valid */

    use(i);
    use(j);
    use(a[0]);
}

/* ===== Switch case bodies ===== */

void switch_case_bodies(int x)
{
    int i = 0;
    int j = 0;

    switch (x) {
        case 0:
            j = i++ + i++;                /* UndCC_Violation - i modified twice */
            break;
        case 1:
            i++;                          /* UndCC_Valid */
            break;
        default:
            j = i++ * i;                  /* UndCC_Violation - i modified and read */
            break;
    }

    use(i);
    use(j);
}

/* ===== Labelled statements ===== */

void labelled_statements(int cond)
{
    int i = 0;
    int j = 0;

    if (!cond) goto end;
    j = i + i++;                          /* UndCC_Violation - i modified and read */

end:
    j = i++;                              /* UndCC_Valid - single modification */
    use(j);
}
