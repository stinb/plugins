// MISRA C 2023

void f(const int *pi)
{
  /* Compliant */
  #define gfun1(X) ( _Generic((X)            \
                  , float32_t: fun1f         \
                  , float64_t: fun1          \
                  , default  : fun1l) (X) )

  #define gfun2(X)   _Generic((X)            \
                  , float32_t: fun2f (X)     \
                  , float64_t: fun2  (X)     \
                  , default  : fun2l (X) )

  /* Non-compliant */
  #define gfun3(X)   _Generic((X) /* UndCC_Violation */ \
                  , float32_t : fun3f (X)               \
                  , float64_t : fun3  (X)               \
                  , float128_t: fun3l (X)               \
                  , default   : default_result )

  /* Compliant by exception */
  #define is_pointer_const(P) _Generic(1 ? (P) : (void *)(P)  \
                            , void const          *: 1        \
                            , void const volatile *: 1        \
                            , default              : 0 )

  _Static_assert (is_pointer_const (pi), "must not be an out-parameter");
}
