// SEI CERT C

#include <string.h>

enum { STR_SIZE = 32 };

// Compliant Solution (Copy without Truncation)

size_t func(const char *source) {
  char c_str[STR_SIZE];
  size_t ret = 0;

  if (source) {
    if (strlen(source) < sizeof(c_str)) {
      strcpy(c_str, source);
      ret = strlen(c_str); // UndCC_FalsePos
    } else {
      /* Handle string-too-large */
    }
  } else {
    /* Handle null pointer */
  }
  return ret;
}
