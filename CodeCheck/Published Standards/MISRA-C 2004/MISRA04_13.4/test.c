#include <stdint.h>

typedef float float32;

float get_f(void);

void fn(void)
{
  int32_t i;
  int32_t n = 10;
  float f;
  double d;
  float32 f32;
  float flimit = 1.0f;
  float *pf = 0;
  float fa[4] = { 0.0f };

  /* --- Compliant: integer controlling expression --- */
  for ( i = 0; i < n; i++ ) { }                   /* UndCC_Valid */
  for ( ; ; ) { break; }                          /* UndCC_Valid - no controlling expression */

  /* --- Non-compliant: floating object in the controlling expression --- */
  for ( f = 0.0f; f < 1.0f; f += 0.1f ) { }       /* UndCC_Violation - f is float */
  for ( d = 0.0; d < 1.0; d += 0.1 ) { }          /* UndCC_Violation - d is double */
  for ( f32 = 0.0f; f32 < 1.0f; f32 += 0.1f ) { } /* UndCC_Violation - typedef of float */
  for ( i = 0; i < n && f < flimit; i++ ) { }     /* UndCC_Violation - float bound in test */

  /* --- Compliant: float only outside the controlling expression --- */
  for ( f = 0.0f; i < n; f += 0.1f ) { }          /* UndCC_Valid - controlling expr has no float */

  /* --- Compliant: not an object of floating type --- */
  for ( i = 0; i < get_f(); i++ ) { }             /* UndCC_Valid - function call, not an object */
  for ( i = 0; pf != 0; i++ ) { }                 /* UndCC_Valid - pf is a pointer, not floating */

  /* --- Non-compliant: floating object reached indirectly --- */
  for ( i = 0; *pf < 1.0f; i++ ) { }              /* UndCC_Violation - *pf is a float object */
  for ( i = 0; pf[i] < 1.0f; i++ ) { }            /* UndCC_Violation - pf[i] is a float object */
  for ( i = 0; fa[i] < 1.0f; i++ ) { }            /* UndCC_Violation - fa[i] is a float object */

  (void) d; (void) f32; (void) f; (void) pf; (void) fa;
}
