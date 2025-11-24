// MISRA C 2023

/* Non-compliant - consists only of a default association */
#define no_op(X) _Generic( (X), default: (X) ) // UndCC_Violation

/* Compliant - has a non-default and a default association */
#define filter_ints(X) ( _Generic( (X)                          \
                       , int32_t: handle_int                    \
                       , default: handle_numeric_value ) (X) )

/* Compliant - has non-default associations */
#define only_ints(X) ( _Generic( (X)                 \
                     , int32_t: handle_int           \
                     , uint32_t: handle_int ) (X) )

/* Compliant - it has a single permitted type and is
               intended to prevent implicit conversion */
#define require_char(X) ( _Generic ( (X), char8_t: (X) ) )
