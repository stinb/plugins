// Custom

#include <cstdint>
#include <iostream>

using namespace std;

typedef int MYINT;

void f02(int *a, int b)
{
  int c = (intptr_t) a; // UndCC_Violation
}

int *f03(int a)
{
  return &a;
}

int main()
{

  int x = 5;
  int *xp = &x;
#if defined(_WIN32) || defined(__APPLE__)
  intptr_t xpc = (intptr_t) xp; // UndCC_Violation(Win,Mac)

  intptr_t xpc2 = (intptr_t) &x;     // UndCC_Violation(Win,Mac)
  intptr_t xpc3 = (intptr_t) f03(x); // UndCC_Violation(Win,Mac)

  f02(xp, reinterpret_cast<intptr_t>(xp)); // UndCC_Violation(Win,Mac)
#endif
  char a = 'b';
  char *ap = &a;

  int xpc5 = (intptr_t) ap;                // UndCC_Violation
#ifdef _WIN32
  int xpc4 = reinterpret_cast<int>(ap);   // UndCC_Violation(Win)
  int xpc6 = (MYINT) ap;                  // UndCC_Violation(Win)
  int xpc7 = reinterpret_cast<MYINT>(ap); // UndCC_Violation(Win)
#endif
  return 0;
}
