// Custom

#include <string.h>

void f0(const char *full)
{
  char *afterHeader = full + 3;
  afterHeader[strlen(afterHeader)-1] = '\0'; // UndCC_Violation
}

void f1(const char *full)
{
  char *afterHeader = full + 3;
  if (0)
    afterHeader[0] = '\0';
  strlen(afterHeader); // UndCC_Violation
}

void f2(const char *full)
{
  char *afterHeader = full + 3;
  afterHeader[0] = '\0';
  strlen(afterHeader); // UndCC_Valid
}
