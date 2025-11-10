#include <stdint.h>

typedef float float32_t;

int32_t x = 0;

/* Non-compliant - not a macro */
int32_t y = _Generic( x            /* UndCC_Violation */
          , int32_t   : 1
          , float32_t : 2 );

/* Compliant - used to implement a generic function */
#define arith(X) ( _Generic( (X)                    \
                 , int32_t   : handle_int32         \
                 , float32_t : handle_float         \
                 , default   : handle_any   ) (X) )

/* Non-compliant */
#define maybe_inc(Y) ( _Generic( x /* UndCC_Violation */ \
                     , int32_t : 1                       \
                     , default : 0 ) + (Y) )
