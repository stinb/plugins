// Custom

typedef union U1 {  // UndCC_Violation
  int i;
  float f;
} U2;               // UndCC_Violation

typedef U1 U3;      // UndCC_Violation
typedef U2 U4;      // UndCC_Violation
using U5 = U1;      // UndCC_Violation
using U6 = U2;      // UndCC_Violation
using U6 = U3;      // UndCC_Violation
using U8 = U4;      // UndCC_Violation
using U9 = union X; // UndCC_Violation

using Int = int;

void f(
  U1 a,             // UndCC_Violation
  U8 b,             // UndCC_Violation
  U9 c,             // UndCC_Violation
  int d,
  Int e);

U1 a;               // UndCC_Violation
U2 b;               // UndCC_Violation
U9 *c;              // UndCC_Violation
int d;
Int e;

struct A
{
  U1 a;             // UndCC_Violation
  U2 b;             // UndCC_Violation
  U9 *c;            // UndCC_Violation
  int d;
  Int e;
};
