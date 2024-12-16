#include <cstdio>

void pass_by_ref(int *x);
void pass_by_value(int x);

void testing()
{
  const int valid1 = 1;
  int const valid2 = 0;
  int valid3 = 1;
  valid3 = valid2 + 1;
  int valid4 = 4;
  pass_by_ref(&valid4);

  int invalid = 1; // UndCC_Violation
  int invalid2; // UndCC_Violation
  invalid2 = 2;
  int invalid3 = 3; // UndCC_Violation
  pass_by_value(invalid3);

}

void pass_by_ref(int *x)
{
  *x = 6;
}

void pass_by_value(int x)
{
  x = 6;
}

void test_param(int invalid4) // UndCC_Violation
{
  char buffer[64];
  sprintf(buffer, "test %i", invalid4);
}
