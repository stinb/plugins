// MISRA C 2012

#include <stdint.h>

enum uint16_t {
  V_MIN = 0,
  V_MAX = 63
};

uint16_t table[64];

int32_t absolute ( int32_t v )
{
  if ( v < 0 )
  {
    return v;
  }
} // UndCC_Violation - control can reach this point without returning a value

uint16_t lookup ( uint16_t v )
{
  if ( ( v < V_MIN ) || ( v > V_MAX ) )
  {
    return; // UndCC_FalseNeg - no value returned. Constraint in C99
  }
  return table[ v ];
}
