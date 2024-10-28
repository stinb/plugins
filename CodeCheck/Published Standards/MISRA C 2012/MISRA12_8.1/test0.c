// MISRA C 2012

#include <stdint.h>

extern void g_0 ( char c, const k ); /* UndCC_Violation */

extern         x_0; /* UndCC_Violation - implicit int type */
extern int16_t x_1; /* UndCC_Valid - explicit type */
const          y_0; /* UndCC_Violation - implicit int type */
const int16_t  y_1; /* UndCC_Valid - explicit type */

extern f_0 ( void );                         /* UndCC_Violation - implicit
                                              * int return type */
extern int16_t f_1 ( void );                 /* UndCC_Valid */

extern void g_1 ( char c, const k );         /* UndCC_Violation - implicit
                                              * int for parameter k */
extern void g_2 ( char c, const int16_t k ); /* UndCC_Valid */

typedef ( *pfi_0 ) ( void );           /* UndCC_Violation - implicit int
                                        * return type */
typedef int16_t ( *pfi_1 ) ( void );   /* UndCC_Valid */
typedef void ( *pfv_0 ) ( const x );   /* UndCC_Violation - implicit int
                                        * for parameter x */
typedef void ( *pfv_1 ) ( int16_t x ); /* UndCC_Valid */

struct str
{
  int16_t x; /* UndCC_Valid */
  const   y; /* UndCC_Violation - implicit int for member y */
} s;
