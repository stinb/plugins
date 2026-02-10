// Custom

#include <cstdint>
#include <iostream>
#define int intptr_t

using namespace std;

typedef intptr_t MYINT;

void f02(int *a, int b)
{
  int c = (intptr_t) a;                      // UndCC_Violation
}

int *f03(int a)
{
  return &a;
}

int fn()
{

  int x = 5;
  int *xp = &x;
  int xpc = (intptr_t) xp;                   // UndCC_Violation

  int xpc2 = (intptr_t) &x;                  // UndCC_Violation
  int xpc3 = (intptr_t) f03(x);              // UndCC_Violation

  f02(xp, reinterpret_cast<intptr_t>(xp));   // UndCC_Violation
  char a = 'b';
  char *ap = &a;
  int xpc4 = reinterpret_cast<intptr_t>(ap); // UndCC_Violation
  int xpc5 = (intptr_t) ap;                  // UndCC_Violation
  int xpc6 = (MYINT) ap;                     // UndCC_Violation
  int xpc7 = reinterpret_cast<MYINT>(ap);    // UndCC_Violation

  return 0;
}
