#include <assert.h>

enum { ZERO = 0 };
const static int ONE = 1;
#define TWO 2

enum { THREE = 3 + 0 };
const static int FOUR = 1L << 2;
#define FIVE '\0' + 5L + '\0'

const static float PI = 3.14159265f;

static void trivial_assertions(int x)
{
  int y = 4;
  if (x)
    y = 5;

  assert(x == 0);
  assert(y == 4);
  assert(!"What?");   // UndCC_Violation - trivial condition
  assert(0);          // UndCC_Violation - trivial condition
  assert(1);          // UndCC_Violation - trivial condition
  assert(!1);         // UndCC_Violation - trivial condition
  assert(!(-1));      // UndCC_Violation - trivial condition
  assert(0 == 0);     // UndCC_Violation - trivial condition
  assert(0 == 1);     // UndCC_Violation - trivial condition
  assert(0 != 0);     // UndCC_Violation - trivial condition
  assert(0 != 1);     // UndCC_Violation - trivial condition
  assert(0 < 0);      // UndCC_Violation - trivial condition
  assert(0 > 1);      // UndCC_Violation - trivial condition
  assert(0 == 1 - 1); // UndCC_Violation - trivial condition
  assert(0 == ZERO);  // UndCC_Violation - trivial condition
  assert(0 == ONE);   // UndCC_Violation - trivial condition
  assert(0 == TWO);   // UndCC_Violation - trivial condition
  assert(0 == THREE); // UndCC_Violation - trivial condition
  assert(0 == FOUR);  // UndCC_Violation - trivial condition
  assert(0 == FIVE);  // UndCC_Violation - trivial condition
  assert(PI == PI);   // UndCC_Violation - trivial condition
}
