// Custom

// This could be defined in the CLI, so flag `strnlen_s` and others
#define __STDC_WANT_LIB_EXT1__ 1

#include <string.h>

void f()
{
  const char s[] = "Hello";
  strnlen_s(s, sizeof(s)); // UndCC_Violation
}
