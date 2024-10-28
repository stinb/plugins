// MISRA C 2012 and MISRA C 2023

#undef __LINE__                  /* UndCC_Violation - begins with _ */
#define _GUARD_H 1               /* UndCC_Violation - begins with _ */
#undef _BUILTIN_sqrt             /* UndCC_Violation - the implementation
                                  * may use _BUILTIN_sqrt for other
                                  * purposes, e.g. generating a sqrt
                                  * instruction */

// #define defined                  /* Analyis error */
#define errno my_errno           /* UndCC_Violation - library identifier */
#define isneg( x ) ( ( x ) < 0 ) /* UndCC_Valid - rule doesn't include
                                  *               future library
                                  *               directions */
