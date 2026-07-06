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

// No pointer/integer conversion occurs in these. The check used to read
// the declared type of an operand while ignoring '&' (address-of) and the
// subscript operator.
void no_conversion(void) {
  uint8_t  dataArr[4] = {0};
  int32_t  state = 2;
  uint8_t  val2;

  uint8_t *dataPtr = &dataArr[0];   // UndCC_Valid  pointer = address of element
  val2 = dataPtr[state] & 0x80;     // UndCC_Valid  subscript dereferences pointer

  int32_t  scalar = 5;
  int32_t *pScalar = &scalar;       // UndCC_Valid  pointer = address of scalar

  const uint8_t *pp = (const uint8_t *)&dataArr[1]; // UndCC_Valid  pointer to pointer
  (void)val2; (void)pScalar; (void)pp;
}

