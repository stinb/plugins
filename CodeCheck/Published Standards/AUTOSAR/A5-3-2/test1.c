#include <stdlib.h>

struct Point {
  int x;
  int y;
};

void dereferencedFirst(int *a, int *b, struct Point *c)
{
  int *d          = malloc(16);
  int *e          = malloc(16);
  struct Point *f = malloc(16);

  a[0] = 1; // UndCC_Violation
  *b   = 2; // UndCC_Violation
  c->x = 3; // UndCC_Violation

  d[0] = 1; // UndCC_Violation
  *e   = 2; // UndCC_Violation
  f->x = 3; // UndCC_Violation
}

void checkedFirst(int *a, int *b, struct Point *c)
{
  int *d          = malloc(16);
  int *e          = malloc(16);
  struct Point *f = malloc(16);

  if (a)
    a[0] = 1; // UndCC_Valid
  if (b)
    *b   = 2; // UndCC_Valid
  if (c)
    c->x = 3; // UndCC_Valid

  if (d)
    d[0] = 1; // UndCC_Valid
  if (e)
    *e   = 2; // UndCC_Valid
  if (f)
    f->x = 3; // UndCC_Valid
}

int main(void)
{
  return 0;
}
