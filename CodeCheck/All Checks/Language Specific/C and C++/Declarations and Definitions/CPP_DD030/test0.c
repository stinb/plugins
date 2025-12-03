// Custom

_Thread_local int *a;
int _Thread_local *b; // UndCC_Violation

extern int *c;
int extern *d; // UndCC_Violation

static int *e;
int static *f; // UndCC_Violation

void function()
{
  register int *g;
  int register *h; // UndCC_Violation
}
