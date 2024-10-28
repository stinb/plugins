#include <stdbool.h>

#define loopBody(n) switch (n) { \
      case 0:                    \
        break;                   \
      case 1:                    \
        break;                   \
      case 2:                    \
        break;                   \
      case 3:                    \
        break;                   \
      case 4:                    \
        break;                   \
    }                            \
                                 \
    break;                       \

void test4 ()
{
  int number = 3;
  bool expression = true;

  do // UndCC_Valid - 1 break
  {
    loopBody(number);
  }
  while (expression);

  do // UndCC_Violation(break, both) - 2 breaks
  {
    if (expression)
      break;

    loopBody(number);
  }
  while (expression);
}
