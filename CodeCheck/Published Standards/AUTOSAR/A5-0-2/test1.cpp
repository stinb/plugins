// Custom

extern int *fn();
extern int fn2();
extern bool takes(bool);
extern int makeInt();

void f(int i, int *p, double d, bool b)
{
  bool init = i;                   // UndCC_Valid - not a condition
  int t = i ? 1 : 2;               // UndCC_Valid - ?: is not a statement condition
  int a = i && 1;                  // UndCC_Valid - not a condition
  switch (i) { default: break; }   // UndCC_Valid - switch is not an iteration statement

  if (i) { }                       // UndCC_Violation
  while (i) { }                    // UndCC_Violation
  for (; i; ) { }                  // UndCC_Violation
  do { } while (i);                // UndCC_Violation
  if (p) { }                       // UndCC_Violation - pointer condition
  if (d) { }                       // UndCC_Violation - floating condition

  if (int len = fn2()) { (void)len; }  // UndCC_Valid - exception, declaration
  if (int *q = fn()) { (void)q; }      // UndCC_Valid - exception, declaration
  for (int x = 0; x < 10; ++x) { }     // UndCC_Valid
  for (;;) { break; }                  // UndCC_Valid - no condition
  if (i != 0) { }                      // UndCC_Valid
  if (b) { }                           // UndCC_Valid
  while (b && i != 0) { }              // UndCC_Valid

  if (static_cast<bool>(i)) { }        // UndCC_Valid - the compliant rewrite
  if ((bool)i) { }                     // UndCC_Valid
  if (takes(i)) { }                    // UndCC_Valid - conversion is the argument's
  if ([&]{ bool inner = i; return inner; }()) { }  // UndCC_Valid - conversion is inside the lambda
  if (!i) { }                          // UndCC_Violation - operand of ! is converted
  if (i ? b : b) { }                   // UndCC_Violation - first operand of ?: is converted
  if (b ? i : 0) { }                   // UndCC_Violation - the condition is not bool

  (void)init; (void)t; (void)a;
}
