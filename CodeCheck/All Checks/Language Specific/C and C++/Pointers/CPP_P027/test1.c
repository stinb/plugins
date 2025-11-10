// Custom

#include <stdio.h>
#include <string.h>

typedef FILE file_t;
typedef file_t file_t_t;

volatile const file_t_t *restrict const volatile a;

void f(const volatile file_t_t *const restrict volatile b)
{
  *a; // UndCC_Violation
  *b; // UndCC_Violation

#ifdef _WIN32
  a->_Placeholder; // UndCC_Violation(win)
  b->_Placeholder; // UndCC_Violation(win)
#elif defined(__linux__)
  a->_fileno;      // UndCC_Violation(lin)
  b->_fileno;      // UndCC_Violation(lin)
#elif defined(__APPLE__)
  a->_file;        // UndCC_Violation(mac)
  b->_file;        // UndCC_Violation(mac)
#endif

  memchr(a, '\0', sizeof(FILE)); // UndCC_Violation
  memcmp(a, b, sizeof(FILE));    // UndCC_Violation
  memcpy(a, b, sizeof(FILE));    // UndCC_Violation
  memmove(a, b, sizeof(FILE));   // UndCC_Violation
  memset(a, '\0', sizeof(FILE)); // UndCC_Violation
}
