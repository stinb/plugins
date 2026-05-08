// C++-specific coverage for MISRA Dir 4.7. The check supports C++ via
// `Function ~Member` filtering; this file verifies that member calls
// with the same name as a default error-returning function are NOT
// flagged, while free / namespace-scoped calls still are.

extern "C" {
    void *malloc(unsigned long);
    int   fclose(void *);
}

class Allocator {
public:
    void *malloc(unsigned long);   // shadows ::malloc as a member
    int   release();
};

void cpp_f(Allocator *a)
{
    void *p;

    // Free function call: discarded statement is flagged.
    malloc(100);                              // UndCC_Violation

    // Captured: compliant.
    p = malloc(100);                          // UndCC_Valid

    // Member function with the same name: excluded by the
    // `Function ~Member` ref filter, so not flagged even when
    // discarded.
    a->malloc(100);                           // UndCC_Valid
    a->release();                             // UndCC_Valid

    // Cast-to-non-void in C++: flagged via the cast-transparent
    // boundary recursion.
    (char *)malloc(100);                      // UndCC_Violation

    // (void) cast: explicit discard, not flagged.
    (void)malloc(100);                        // UndCC_Valid

    // Tested in a controlling expression: compliant.
    if (fclose(p) == 0) { }                   // UndCC_Valid

    (void)p;
}
