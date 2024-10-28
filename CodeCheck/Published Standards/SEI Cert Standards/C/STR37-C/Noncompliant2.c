#include <ctype.h>
#include <string.h>

size_t alpha_test(const char *s)
{
  const char *t = s;
  isalpha(*t); // UndCC_Violation
  return t - s;
}
