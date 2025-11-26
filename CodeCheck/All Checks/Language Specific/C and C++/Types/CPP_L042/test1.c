// Custom


#include <stdio.h>


void types_fgetc(FILE *file)
{
  short s;
  int i;
  long l;
  long long ll;
  unsigned short us;
  unsigned u;
  unsigned long ul;
  unsigned long long ull;

  s = fgetc(file);
  i = fgetc(file);
  l = fgetc(file);
  ll = fgetc(file);
  us = fgetc(file);
  u = fgetc(file);
  ul = fgetc(file);
  ull = fgetc(file);

  s == EOF;   // UndCC_Violation
  i == EOF;
  l == EOF;
  ll == EOF;
  us == EOF;  // UndCC_Violation
  u == EOF;   // UndCC_Violation
  ul == EOF;  // UndCC_Violation
  ull == EOF; // UndCC_Violation
}


void types_getc(FILE *file)
{
  short s;
  int i;
  long l;
  long long ll;
  unsigned short us;
  unsigned u;
  unsigned long ul;
  unsigned long long ull;

  s = getc(file);
  i = getc(file);
  l = getc(file);
  ll = getc(file);
  us = getc(file);
  u = getc(file);
  ul = getc(file);
  ull = getc(file);

  s == EOF;   // UndCC_Violation
  i == EOF;
  l == EOF;
  ll == EOF;
  us == EOF;  // UndCC_Violation
  u == EOF;   // UndCC_Violation
  ul == EOF;  // UndCC_Violation
  ull == EOF; // UndCC_Violation
}


void types_getchar(void)
{
  short s;
  int i;
  long l;
  long long ll;
  unsigned short us;
  unsigned u;
  unsigned long ul;
  unsigned long long ull;

  s = getchar();
  i = getchar();
  l = getchar();
  ll = getchar();
  us = getchar();
  u = getchar();
  ul = getchar();
  ull = getchar();

  s == EOF;   // UndCC_Violation
  i == EOF;
  l == EOF;
  ll == EOF;
  us == EOF;  // UndCC_Violation
  u == EOF;   // UndCC_Violation
  ul == EOF;  // UndCC_Violation
  ull == EOF; // UndCC_Violation
}


void direct(FILE *file)
{
  fgetc(file) == EOF;
  fgetc(file) != EOF;
  EOF == fgetc(file);
  EOF != fgetc(file);

  getc(file) == EOF;
  getc(file) != EOF;
  EOF == getc(file);
  EOF != getc(file);

  getchar() == EOF;
  getchar() != EOF;
  EOF == getchar();
  EOF != getchar();
}
