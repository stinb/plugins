#include <stdlib.h>

static void f()
{
  char str[256];

  // Calls to all four functions
  atof(str);  // UndCC_Violation
  atoi(str);  // UndCC_Violation
  atol(str);  // UndCC_Violation
  atoll(str); // UndCC_Violation(0) - configured by option

  // Any reference
  int (*fn)(const char *str);
  fn = atoi;  // UndCC_Violation
  fn(str);

  // Macro expansion
  #define FN(_str) atoi(_str);
  FN(str);    // UndCC_Violation
}
