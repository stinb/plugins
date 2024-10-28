// MISRA C 2012


typedef double float64_t;


/*
 * Include <stddef.h> to define size_t
 */
#include <stddef.h>
extern void *memcpy ( void *restrict s1, const void *restrict s2, size_t n ); /* UndCC_Violation */


extern double sqrt ( double x ); /* UndCC_Violation */

#define sqrt( x ) ( _BUILTIN_sqrt ( x ) ) /* UndCC_Violation */


#define _BUILTIN_sqrt( x ) ( x ) /* UndCC_Violation */
// #include <math.h> // Analysis error because of sqrt and _BUILTIN_sqrt macros

float64_t x = sqrt ( ( float64_t ) 2.0 ); /* sqrt may not behave as
                                           * defined in The Standard */
