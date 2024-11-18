#include <stdint.h>
int main() {
  uint32_t counter = 0u;
  for ( float f = 0.0f; f < 1.0f; f += 0.001f ) // UndCC_Violation
  {
    ++counter;
  }
  float f;
  for ( uint32_t counter = 0u; counter < 1000u; ++counter ) // UndCC_Valid
  {
    f = ( float ) counter * 0.001f;
  }
  f = 0.0f;     
  while ( f < 1.0f ) // UndCC_Violation
  {
    f += 0.001f;
  }
  uint32_t u32a;
  f = read_float32 ( );
  do
  {
    u32a = read_u32 ( );
    /* f does not change in the loop so cannot be a loop counter */
  } while ( ( ( float ) u32a - f ) > 10.0f ); // UndCC_Valid
  do
  {
    f += 0.001f;
  } while ( f < 1.0f ); // UndCC_Violation

}

