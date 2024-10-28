#include <stdint.h>
int main() {
  int16_t a1[ 5 ] = { [ 0 ] = -5, [ 1 ] = -4, [ 2 ] = -3, [ 2 ] = -2, [ 4 ] = -1 }; // UndCC_Valid
  int16_t a2[ ] = { [ 0 ] = -5, [ 1 ] = -4, [ 2 ] = -3, [ 2 ] = -2, [ 4 ] = -1 }; // UndCC_Violation

  int a3[ ] = { [ 0 ] = 1 }; // UndCC_Violation
  int a4[ 1 ] = { [ 0 ] = 1 }; // UndCC_Valid
  int a5[ 10 ] = { [ 0 ] = 1 }; // UndCC_Valid
}

