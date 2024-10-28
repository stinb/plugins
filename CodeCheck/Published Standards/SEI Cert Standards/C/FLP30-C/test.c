#include <stddef.h>


void func(void) {
  for (float x = 0.1f; x <= 1.0f; x += 0.1f) { // UNDCC_Violation
    /* Loop may iterate 9 or 10 times */
  }
}

// UndCC_Valid
void func1(void) {
  for (size_t count = 1; count <= 10; ++count) {
    float x = count / 10.0f;
    /* Loop iterates exactly 10 times */
  }
}


void func2(void) {
  for (float x = 100000001.0f; x <= 100000010.0f; x += 1.0f) { // UNDCC_Violation
    /* Loop may not terminate */
  }
}

// UndCC_Valid
void func3(void) {
  for (size_t count = 1; count <= 10; ++count) {
    float x = 100000000.0f + (count * 1.0f);
    /* Loop iterates exactly 10 times */
  }
}
