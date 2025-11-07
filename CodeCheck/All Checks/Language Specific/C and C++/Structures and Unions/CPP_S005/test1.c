// Custom

#include <stdint.h>
#include <string.h>

struct Vec
{
  int x;
  int y;
};

void f()
{
  uint8_t a;
  int8_t b;
  unsigned char c;
  signed char d;
  _Bool e;
  int f;
  unsigned g;
  int *h[10];

  char i;
  struct Vec j;

  memcmp(&a, &a, sizeof(a));
  memcmp(&b, &b, sizeof(b));
  memcmp(&c, &c, sizeof(c));
  memcmp(&d, &d, sizeof(d));
  memcmp(&e, &e, sizeof(e));
  memcmp(&f, &f, sizeof(f));
  memcmp(&g, &g, sizeof(g));
  memcmp(h, h, sizeof(h));

  memcmp(&i, &i, sizeof(i)); // UndCC_Violation
  memcmp(&j, &j, sizeof(j)); // UndCC_Violation
}
