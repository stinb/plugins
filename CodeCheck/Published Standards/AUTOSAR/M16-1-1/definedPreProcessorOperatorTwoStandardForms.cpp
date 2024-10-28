#ifdef __UNDERSTAND_FUZZY_PARSE__
#if defined(X > Y) // UndCC_Violation – constraint violation
#endif
#endif

#define DEFINED defined
#if DEFINED(X) // UndCC_Violation
#endif
#if defined A // UndCC_Valid
#endif
#if defined(B) // UndCC_Valid
#endif

// MISRA19.1.1

#ifdef __UNDERSTAND_FUZZY_PARSE__
#if defined 1 // UndCC_Violation - 1 is not an
              // identifier
#endif
#endif

#define FEATURE(x) defined(x) && (x != 0)
#if FEATURE(X) // UndCC_Violation - defined resulting
               // from expansion
#endif

#if defined(x) && (x != 0) // UndCC_Valid
#endif
