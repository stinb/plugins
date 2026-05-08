#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern void use(void *);
extern void use_int(int);

void f(void)
{
    void *p;
    int rc;
    FILE *fp;

    // Statement-only call with discarded return: non-compliant.
    malloc(100);                          // UndCC_Violation
    fopen("a", "r");                      // UndCC_Violation
    fclose(NULL);                         // UndCC_Violation
    printf("hi\n");                       // UndCC_Violation
    fseek(NULL, 0, 0);                    // UndCC_Violation

    // Result assigned: compliant — caller can subsequently test it.
    p = malloc(100);                      // UndCC_Valid
    fp = fopen("a", "r");                 // UndCC_Valid
    rc = fclose(fp);                      // UndCC_Valid

    // Result tested in a controlling expression: compliant.
    if (malloc(100) == NULL) { use_int(1); }   // UndCC_Valid
    if (fopen("b", "r")) { use_int(2); }       // UndCC_Valid
    while (fread(p, 1, 1, fp) == 1) { }        // UndCC_Valid

    // Result used in a wider expression: compliant.
    use(malloc(100));                          // UndCC_Valid
    use_int(printf("x") + 1);                  // UndCC_Valid

    // Cast-to-void: explicit-discard idiom — accepted as compliant
    // (Rule 17.7 deviation), though Dir 4.7 strictly requires testing.
    (void)malloc(100);                         // UndCC_Valid

    // Cast to non-void with discarded result: still a violation.
    // The boundary check looks through the cast.
    (char *)malloc(100);                       // UndCC_Violation
    (FILE *)fopen("d", "r");                   // UndCC_Violation

    // Statement-only call as the body of an if/else without braces.
    if (rc) malloc(100);                       // UndCC_Violation
    else fopen("c", "r");                      // UndCC_Violation

    // Body of a do/while without braces.
    do malloc(100); while (0);                 // UndCC_Violation

    // free returns void, never reported.
    free(p);                                   // UndCC_Valid

    // Functions outside the configured list are not flagged even if
    // their return value is discarded.
    strlen("test");                            // UndCC_Valid

    // Result used in a ternary: compliant — the call's value is the
    // expression result.
    p = (rc) ? malloc(100) : NULL;             // UndCC_Valid

    // for-loop init / update clauses: both discard the call's
    // result. Neither is flagged: the init's previous token is '('
    // and the update's following token is ')', so neither side of
    // the boundary check matches.
    for (malloc(100); rc < 1; rc++) { }        // UndCC_FalseNeg
    for (rc = 0; rc < 1; malloc(100)) { }      // UndCC_FalseNeg

    // Labeled / case-labeled statement-only calls. The lexer-based
    // boundary check sees ':' before the call and treats it as
    // non-boundary (same token as a ternary ':'), so these are not
    // flagged.
    switch (rc) {
        case 1: malloc(100); break;            // UndCC_FalseNeg
        default: break;
    }
    goto lbl;
lbl: malloc(100);                              // UndCC_FalseNeg
}

void *return_alloc(void)
{
    // Result used as the return value: compliant.
    return malloc(100);                        // UndCC_Valid
}
