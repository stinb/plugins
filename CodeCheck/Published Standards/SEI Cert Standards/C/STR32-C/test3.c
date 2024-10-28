// SEI CERT C

#define __STDC_WANT_LIB_EXT1__ 1
#include <string.h>

enum { STR_SIZE = 32 };

// Compliant Solution (Truncation, strncpy_s())

size_t func(const char *source) {
  char a[STR_SIZE];
  size_t ret = 0;

  if (source) {
    int err = strncpy_s(
      a, sizeof(a), source, strlen(source)
    );
    if (err != 0) {
      /* Handle error */
    } else {
      ret = strnlen_s(a, sizeof(a));
    }
  } else {
     /* Handle null pointer */
  }
  return ret;
}
