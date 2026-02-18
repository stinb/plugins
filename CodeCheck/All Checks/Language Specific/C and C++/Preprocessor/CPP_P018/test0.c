// MISRA C 2012 & 2023

#define int some_other_type                          // UndCC_Violation
#undef int                                           // Prevent analysis errors
#include <stdlib.h>

#define while( E ) for ( ; ( E ) ; )                 // UndCC_Violation - redefined while
#define unless( E ) if ( ! ( E ) )                   // UndCC_Valid

#define seq( S1, S2 ) do { S1; S2; } while ( false ) // UndCC_Valid
#define compound( S ) { S; }                         // UndCC_Valid

/* Remove inline if compiling for C90 */
#define inline                                       // UndCC_Valid(c89)
