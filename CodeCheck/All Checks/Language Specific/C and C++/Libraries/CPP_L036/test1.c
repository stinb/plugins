// Custom

#include <ctype.h>

void f()
{
  isalpha(-1); // UndCC_Violation
  isalpha(0u);
  isalpha(255u);
  isalpha(256u); // UndCC_Violation

  isalpha(0x0);
  isalpha(0xff);
  isalpha(0xff1); // UndCC_Violation
}
