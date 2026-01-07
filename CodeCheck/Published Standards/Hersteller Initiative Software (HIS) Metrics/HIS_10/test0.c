// Custom

#define MAX 10
#define NULL 0

int multipleExits(unsigned n, char *p) // UndCC_Violation
{
  if (n > MAX)
    return 0;

  if (p == NULL)
    return 0;

  return 1;
}

int oneExit(unsigned n, char *p) // UndCC_Valid
{
  int ret = 1;

  if (n > MAX)
    ret = 0;

  else if (p == NULL)
    ret = 0;

  return ret;
}
