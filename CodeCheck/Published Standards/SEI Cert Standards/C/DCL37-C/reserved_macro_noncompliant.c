// SEI CERT C++


#include <inttypes.h>
#include <stdio.h>

#undef SIZE_MAX // NOTE: this is a fix for a compilation error

static const int_fast16_t INTFAST16_LIMIT_MAX = 12000;  // UndCC_Violation

void print_fast16(int_fast16_t val) {
  enum { SIZE_MAX = 80 };  // UndCC_Violation
  char buf[SIZE_MAX];
  if (INTFAST16_LIMIT_MAX < val) {
    sprintf(buf, "The value is too large");
  } else {
    snprintf(buf, SIZE_MAX, "The value is %" PRIdFAST16, val);
  }
}
