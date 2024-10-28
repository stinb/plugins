// Custom

#include <cstdint>
#include <iostream>

using namespace std;

typedef int MYINT;

void f02(int *a, int b)
{
  int c = (int64_t) a; // UndCC_Violation
}

int *f03(int a)
{
  return &a;
}

int main()
{

  int x = 5;
  int *xp = &x;

  int xpc = (int) xp; // UndCC_Violation

  int xpc2 = (int) &x;     // UndCC_Violation
  int xpc3 = (int) f03(x); // UndCC_Violation

  f02(xp, reinterpret_cast<int32_t>(xp)); // UndCC_Violation

  char a = 'b';
  char *ap = &a;

  int xpc4 = reinterpret_cast<int>(ap);   // UndCC_Violation
  int xpc5 = (int64_t) ap;                // UndCC_Violation
  int xpc6 = (MYINT) ap;                  // UndCC_Violation
  int xpc7 = reinterpret_cast<MYINT>(ap); // UndCC_Violation

  return 0;
}
