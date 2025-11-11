// Custom

#include <stdio.h>

void f(void)
{
  FILE *fp;

  fp = fopen("tmp", "w");

  for (int i = 0; i < 10; i++) {
    if (i == 9)
      fclose ( fp );
    fprintf (fp, "?"); // UndCC_Violation
  }
}

void g(void)
{
  FILE *fp;

  fp = fopen("tmp", "w");

  for (int i = 0; i < 10; i++) {
    switch (i) {
      case 9:
        fprintf(fp, "?"); // UndCC_Violation
        break;
      case 8:
        fclose(fp);
        break;
    }
  }
}

void h(void)
{
  FILE *fp;

  fp = fopen("tmp", "w");

  for (int i = 0; i < 10; i++) {
    switch (i) {
      case 9:
        fprintf(fp, "?"); // UndCC_Valid
        break;
    }
  }

  fclose(fp);
}

void i(void)
{
  FILE *fp;

  fp = fopen("tmp", "w");
  goto CLOSE;

RETURN:
  fprintf(fp, "?"); // UndCC_Violation
  return;

CLOSE:
  fclose(fp);
  goto RETURN;
}
