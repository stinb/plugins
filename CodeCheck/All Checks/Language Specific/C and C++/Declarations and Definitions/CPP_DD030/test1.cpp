// Custom

thread_local int *a;
int thread_local *b; // UndCC_Violation

extern int *c;
int extern *d; // UndCC_Violation

static int *e;
int static *f; // UndCC_Violation

struct Struct
{
  mutable int *g;
  int mutable *h; // UndCC_Violation
};
