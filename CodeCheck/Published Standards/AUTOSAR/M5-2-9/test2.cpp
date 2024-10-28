// Custom

#include <cstdint>
#include <iostream>

using namespace std;

typedef int MYINT;

void f02(int *a, int b)
{
  int c = (int64_t) a; // UndCC_Violation(LIN,WIN)
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
  int xpc = (int) xp; // UndCC_Violation(WIN)

  int xpc2 = (int) &x;     // UndCC_Violation(WIN)
  int xpc3 = (int) f03(x); // UndCC_Violation(WIN)

  f02(xp, reinterpret_cast<int32_t>(xp)); // UndCC_Violation(WIN)

  char a = 'b';
  char *ap = &a;

  int xpc4 = reinterpret_cast<int>(ap);   // UndCC_Violation(WIN)
  int xpc5 = (int64_t) ap;                // UndCC_Violation(WIN)
  int xpc6 = (MYINT) ap;                  // UndCC_Violation(WIN)
  int xpc7 = reinterpret_cast<MYINT>(ap); // UndCC_Violation(WIN)
#endif
  return 0;
}
