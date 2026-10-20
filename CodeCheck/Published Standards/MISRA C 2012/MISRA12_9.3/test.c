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

void issue_cases(void)
{
  int32_t a1[ 4 ][ 4 ] = { 0 }; // UndCC_Valid - exception 1 covers the whole array object
  int32_t a2[] = { // UndCC_Valid - size comes from the initializer
    #include "test_data.h"
  };
  int32_t a3[] = { 1, 2, 3 }; // UndCC_Valid
  int32_t a4[ 3 ] = { 10 }; // UndCC_Violation - not the { 0 } form
  int32_t a5[ 5 ] = { 100, 200 }; // UndCC_Violation
  int32_t a6[ 2 ][ 3 ] = { { 0 }, { 1, 2, 3 } }; // UndCC_Valid
  int32_t a7[ 2 ][ 2 ][ 2 ] = { 0 }; // UndCC_Valid
  int32_t a8[ 2 ][ 2 ] = { { 0 } }; // UndCC_Violation - a8[ 1 ] is implicitly initialized
  int32_t a9[ 2 ][ 2 ] = { 1, 2, 3, 4 }; // UndCC_Valid
  int32_t b1[ 4 ] = { 0, 0 }; // UndCC_Violation
  int32_t b2[][ 2 ] = { 1, 2, 3 }; // UndCC_Violation - b2[ 1 ][ 1 ] is implicitly initialized
  char    b3[ 2 ][ 6 ] = { "hello", "world" }; // UndCC_Valid - exception 3
  char    b4[ 6 ] = { "hi" }; // UndCC_Valid - exception 3
  int32_t b5[ 3 ] = { 1, 2, 3, }; // UndCC_Valid - trailing comma
}

void zero_forms(void)
{
  char     c1[ 8 ] = { '\0' }; // UndCC_Valid - zero-valued literal
  float    f1[ 4 ] = { 0.0f }; // UndCC_Valid
  float    f2[ 2 ][ 2 ] = { 0.0 }; // UndCC_Valid
  uint32_t u1[ 4 ] = { 0U }; // UndCC_Valid
  uint32_t u2[ 4 ] = { 0x0 }; // UndCC_Valid
  uint32_t u3[ 4 ] = { 10U }; // UndCC_Violation
  float    f3[ 4 ] = { 1.0f }; // UndCC_Violation
}
