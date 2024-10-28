// SEI CERT C

#include <string.h>

enum { STR_SIZE = 32 };

// Noncompliant Code Example (strncpy())

size_t func_noncompliant(const char *source) {
  char c_str[STR_SIZE];
  size_t ret = 0;

  if (source) {
    c_str[sizeof(c_str) - 1] = '\0';
    strncpy(c_str, source, sizeof(c_str));
    ret = strlen(c_str); // UndCC_Violation
  } else {
    /* Handle null pointer */
  }
  return ret;
}

// Compliant Solution (Truncation)

size_t func_compliant(const char *source) {
  char c_str[STR_SIZE];
  size_t ret = 0;

  if (source) {
    strncpy(c_str, source, sizeof(c_str) - 1);
    c_str[sizeof(c_str) - 1] = '\0';
    ret = strlen(c_str);
  } else {
    /* Handle null pointer */
  }
  return ret;
}
