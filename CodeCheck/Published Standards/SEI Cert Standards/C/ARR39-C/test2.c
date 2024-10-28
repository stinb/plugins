// SEI CERT C

#include <string.h>
#include <stdlib.h>
#include <stddef.h>

#ifndef offsetof
#define offsetof __builtin_offsetof
#endif

struct big {
  unsigned long long ull_a;
  unsigned long long ull_b;
  unsigned long long ull_c;
  int si_e;
  int si_f;
};

void func(void) {
  size_t skip = offsetof(struct big, ull_b);
  struct big *s = (struct big *)malloc(sizeof(struct big));
  if (s == NULL) {
    /* Handle malloc() error */
  }

  memset(s + skip, 0, sizeof(struct big) - skip); // UndCC_Violation - skip is scaled
  /* ... */
  free(s);
  s = NULL;
}
