#include <stdint.h>
#include <math.h>

#ifndef M_PI
  #define M_PI 3.14159265358979323846
#endif

int main() {
  int32_t x[ 3 ] = { 0, 1, 2 }; // UndCC_Valid
  int32_t y[ 3 ] = { 0, 1 }; // UndCC_Violation
  float t[ 4 ] = { [ 1 ] = 1.0f, 2.0f }; // UndCC_Violation
  float z[ 50 ] = { [ 1 ] = 1.0f, [ 25 ] = 2.0f }; // UndCC_Valid
  float arr[ 3 ][ 2 ] = // UndCC_Valid
  {
    { 0.0f, 0.0f },
    { M_PI / 4.0f, -M_PI / 4.0f },
    { 0 } // initializes all elements of array subobject arr[ 2 ]
  };
  char h[ 10 ] = "Hello"; // UndCC_Valid by Exception 3
}
