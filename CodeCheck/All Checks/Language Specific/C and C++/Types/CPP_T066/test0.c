// MISRA C 2023

/* Non-compliant - default is not first or last association */
#define sqrt(X) ( _Generic((X)                           \
                , float32_t : sqrtf                      \
                , default   : sqrt /* UndCC_Violation */ \
                , float128_t: sqrtl) (X) )

/* Compliant - default is first association */
#define cbrt(X) ( _Generic((X)             \
                , default   : cbrt         \
                , float32_t : cbrtf        \
                , float128_t: cbrtl) (X) )

/* Compliant - no default */
#define assert_untyped_nonatomic(X) ( _Generic((X)                            \
                                    , void                * : handle_ptr      \
                                    , void const          * : handle_ptr      \
                                    , void volatile       * : handle_ptr      \
                                    , void const volatile * : handle_ptr) (X)
