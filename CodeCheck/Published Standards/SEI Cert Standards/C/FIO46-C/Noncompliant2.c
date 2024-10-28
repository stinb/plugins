#include <stdio.h>

int close_stdin(void)
{
  char c;
  if (fclose(stdin) == EOF)
  {
    return -1;
  }

  c = getc(stdin); // UNDCC_Violation, stdin is accessed after closing
  return 0;
}

int main()
{
  FILE *fp;
  char c;

  fp = fopen("file.txt", "w");

  fprintf(fp, "%s", "This is tutorialspoint.com");
  fclose(fp);

  ungetc(c, fp); // UNDCC_Violation, fp is accessed after closing

  return (0);
}
