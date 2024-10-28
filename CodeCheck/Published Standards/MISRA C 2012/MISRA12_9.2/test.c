#include <stdint.h>

void a () {
  int16_t y[ 3 ][ 2 ] = { 1, 2, 0, 0, 5, 6 }; /* UndCC_Violation */
  int16_t y1[ 3 ][ 2 ] = { { 1, 2 }, { 0 }, { 5, 6 } }; /* UndCC_Valid */
  int test[2][3][4] = { 
    { {3, 4, 2, 3}, {0, -3, 9, 11}, {23, 12, 23, 2} },
    { {13, 4, 56, 3}, {5, 9, 3, 5}, {5, 1, 4, 9} }
  };
  int16_t y2[ 3 ][ 2 ] = { { 1, 2 }, { 0, 0 }, { 5, 6 } }; /* UndCC_Valid */
  int16_t z1[ 2 ][ 2 ] = { { 0 }, [ 1 ][ 1 ] = 1 }; /* UndCC_Valid */
  int16_t z2[ 2 ][ 2 ] = { { 0 },
    [ 1 ][ 1 ] = 1, [ 1 ][ 0 ] = 0
  }; /* UndCC_Valid */
  int16_t z3[ 2 ][ 2 ] = { { 0 }, [ 1 ][ 0 ] = 0, 1 }; /* UndCC_FalseNeg */
  int16_t z4[ 2 ][ 2 ] = { [ 0 ][ 1 ] = 0, { 0, 1 } }; /* UndCC_Valid */
  float a[ 3 ][ 2 ] = { 0 }; /* UndCC_Valid */
  float a1[ 3 ][ 2 ] = { { 0 }, { 0 }, { 0 } }; /* UndCC_Valid */
  float a2[ 3 ][ 2 ] = { { 0.0f, 0.0f },
    { 0.0f, 0.0f },
    { 0.0f, 0.0f }
  }; /* UndCC_Valid */
  union u1 {
    int16_t i;
    float f;
  } u = { 0 }; /* UndCC_Valid */
  struct s1 {
    uint16_t len;
    char buf[ 8 ];
  } s[ 3 ] = {
    { 5u, { 'a', 'b', 'c', 'd', 'e', '\0', '\0', '\0' } },
    { 2u, { 0 } },
    { .len = 0u } /* UndCC_Valid - buf initialized implicitly */
  }; /* UndCC_Valid - s[] fully initialized */
}

