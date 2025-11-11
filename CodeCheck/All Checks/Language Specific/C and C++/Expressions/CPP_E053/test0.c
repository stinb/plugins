// MISRA C 2023

#include <stdint.h>

#ifdef BIG
typedef int64_t STATE;
#else
typedef int16_t STATE;
#endif

STATE shared_state;

_Static_assert ( _Generic ( shared_state // UndCC_Valid
               , int32_t: 0
               , default: 1 )
               , "error on wrong type");

_Static_assert ( _Generic ( ++shared_state // UndCC_Violation
               , int32_t: 0
               , default: 1)
               , "error on wrong type");
