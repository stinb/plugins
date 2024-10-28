// MISRA C++ 2008

#include <cstdint>

typedef int16_t ( * CallbackFn )( int16_t a, int16_t b );
int16_t Callback_1 ( int16_t a, int16_t b ) // UndCC_Valid
{
  return a + b;
}
int16_t Callback_2 ( int16_t a, int16_t b ) // UndCC_Violation
{
  return a;
}
int16_t Callback_3 ( int16_t, int16_t b )   // UndCC_Valid by exception
{
  return b;
}
void Dispatch ( int16_t n,
                int16_t a,
                int16_t b,
                int16_t c, // UndCC_Violation
                int16_t )  // UndCC_Violation if Dispatch not a callback
{
  CallbackFn pFn;
  switch ( n )
  {
    case  0: pFn = &Callback_1; break;
    case  1: pFn = &Callback_2; break;
    default: pFn = &Callback_3; break;
  }
  ( *pFn )( a, b );
}
