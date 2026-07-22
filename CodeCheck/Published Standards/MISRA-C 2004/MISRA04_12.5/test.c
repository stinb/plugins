#include <stdint.h>

typedef struct { int32_t b; } s_t;

int32_t is_odd(int32_t y);

#define A 1
#define B 0

void fn(int32_t x, int32_t y, int32_t z, int32_t ishigh,
        int32_t c1, int32_t c2, int32_t c3, s_t a, s_t *p, s_t *q)
{
  int32_t r = 0;

  /* --- Examples from MISRA C 2004 Rule 12.5 --- */
  if ( ( x == 0 ) && ishigh ) { r = 1; }                    /* UndCC_Valid - x == 0 made primary */
  if ( x || y || z ) { r = 1; }                             /* UndCC_Valid - sequence of only || */
  if ( x || ( y && z ) ) { r = 1; }                         /* UndCC_Valid - y && z made primary */
  if ( x && ( !y ) ) { r = 1; }                             /* UndCC_Valid - !y made primary */
  if ( ( is_odd(y) ) && x ) { r = 1; }                      /* UndCC_Valid - call made primary */
  if ( ( x > c1 ) && ( y > c2 ) && ( z > c3 ) ) { r = 1; }  /* UndCC_Valid - sequence of only && */
  if ( ( x > c1 ) && ( ( y > c2 ) || ( z > c3 ) ) ) { r = 1; } /* UndCC_Valid - extra () used */

  /* --- The non-compliant counterparts --- */
  if ( x == 0 && ishigh ) { r = 1; }                        /* UndCC_Violation - x == 0 not primary */
  if ( x || y && z ) { r = 1; }                             /* UndCC_Violation - y && z not primary */
  if ( x && !y ) { r = 1; }                                 /* UndCC_Violation - !y not primary */
  if ( is_odd(y) && x ) { r = 1; }                          /* UndCC_Violation - call not primary */
  if ( ( x > c1 ) && ( y > c2 ) || ( z > c3 ) ) { r = 1; }  /* UndCC_Violation - mixed && and || */

  /* --- Common cases --- */
  if ( x && y ) { r = 1; }                                  /* UndCC_Valid - single identifiers */
  if ( x && y && z ) { r = 1; }                             /* UndCC_Valid - sequence of only && */
  if ( a.b && x ) { r = 1; }                                /* UndCC_Violation - member access not primary */
  if ( p->b && q->b ) { r = 1; }                            /* UndCC_Violation - member access not primary */
  if ( ( a.b ) && ( p->b ) ) { r = 1; }                     /* UndCC_Valid - members made primary */
  if ( x + 1 && y ) { r = 1; }                              /* UndCC_Violation - x + 1 not primary */
  if ( x && y == z ) { r = 1; }                             /* UndCC_Violation - y == z not primary */

  /* --- Preprocessor conditionals are out of scope (Rule 19.x) --- */
#if defined(A) && defined(B)                                /* UndCC_Valid - preprocessor line */
  r = 2;
#endif

  /* --- Code adjacent to a directive is still in scope --- */
#if defined(A)
  if ( x == 0 && y ) { r = 1; }                             /* UndCC_Violation - first statement in #if block */
#endif
  if ( x == 0 && z ) { r = 1; }                             /* UndCC_Violation - first statement after #endif */

  (void) r;
}
