#include <stdint.h>
#include <stdbool.h>
int main() {
  uint8_t *PORTA = ( uint8_t * ) 0x0002; // UndCC_FalseNeg
  uint16_t *p;
  int32_t addr = ( int32_t ) &p; // UndCC_Violation
  uint8_t *q = ( uint8_t * ) addr; // UndCC_Violation
  bool b = ( bool ) p; // UndCC_Violation
  enum etag { A, B } e = ( enum etag ) p; // UndCC_FalseNeg
}

