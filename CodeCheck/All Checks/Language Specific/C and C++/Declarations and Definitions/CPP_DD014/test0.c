#include <stdint.h>
#include <stdbool.h>
#define bool_t bool

// MISRA C 2012

extern void f_0 ( signed int );  // UndCC_FalseNeg - Exception allows the same primitive types
       void f_0 (        int );  // UndCC_FalseNeg - Exception allows the same primitive types
extern void g_0 ( int * const ); // UndCC_Violation(a) - Type qualifier const
       void g_0 ( int *       ); // UndCC_Violation(a) - No type qualifier const


extern int16_t func_0 ( int16_t num, int16_t den );  // UndCC_Violation(a) - Parameter names different

int16_t func_0 ( int16_t den, int16_t num )          // UndCC_Violation(a) - Parameter names different
{
  return num / den;
}


typedef uint16_t width_t;
typedef uint16_t height_t;
typedef uint32_t area_t;

extern area_t area_0 ( width_t w, height_t h ); // UndCC_Violation(a) - Parameter types different

area_t area_0 ( width_t w, width_t h )          // UndCC_Violation(a) - Parameter types different
{
  return ( area_t ) w * h;
}


extern void f1_0 ( int16_t x );
extern void f2_0 ( int16_t y );

void h_0 ( bool_t b )
{
  void ( *fp1 ) ( int16_t z ) = b ? f1_0 : f2_0;
}
