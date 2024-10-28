#include <stdbool.h>

void test3 ()
{
  int number = 3;
  bool expression = true;

  do // UndCC_Valid - 1 break
  {
    switch (number) {
      case 0:
        break;
      case 1:
        break;
      case 2:
        break;
      case 3:
        break;
      case 4:
        break;
    }

    break;
    break; // This break is not reachable in the control-flow
  }
  while (expression);
}
