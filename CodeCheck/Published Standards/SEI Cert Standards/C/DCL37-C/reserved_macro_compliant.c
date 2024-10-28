// SEI CERT C++


#include <inttypes.h>
#include <stdio.h>

static const int_fast16_t MY_INTFAST16_UPPER_LIMIT = 12000;

void print_fast16(int_fast16_t val) {
  enum { BUFSIZE = 80 };
  char buf[BUFSIZE];
  if (MY_INTFAST16_UPPER_LIMIT < val) {
    sprintf(buf, "The value is too large");
  } else {
    snprintf(buf, BUFSIZE, "The value is %" PRIdFAST16, val);
  }
}
