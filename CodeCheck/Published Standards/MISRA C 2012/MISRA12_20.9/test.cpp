#if M == 0                              /* UndCC_Violation - M is not a macro */
#endif

#if defined( N )                        /* UndCC_Valid - N is not evaluated */
#if N == 0                              /* UndCC_Valid - N is defined here */
#endif
#endif

#if defined( B ) && ( B == 0 )          /* UndCC_Valid - B is only evaluated if defined */
#endif

#if !defined( P )                       /* UndCC_Valid */
#endif

#define K 1
#if K == 1                              /* UndCC_Valid */
#elif Q == 2                            /* UndCC_Violation - Q is not a macro */
#endif
