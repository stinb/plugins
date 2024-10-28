#if x < 0 // UndCC_Violation - x not defined
#endif

#ifdef __UNDERSTAND_FUZZY_PARSE__
#if defined ( X > Y ) // UndCC_Valid
#endif
#endif

#define DEFINED defined
#if DEFINED(X) // UndCC_Violation
#endif

#ifdef __UNDERSTAND_FUZZY_PARSE__
#defined A // UndCC_Valid
#endif

#if defined ( B ) // UndCC_Valid
#endif
