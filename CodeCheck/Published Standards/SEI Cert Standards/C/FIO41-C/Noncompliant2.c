
#include <stdio.h>

void func_4(const char *file_name)
{
  FILE *fptr = NULL;
  int c = 'a';

  while (c <= 'z')
  {
    if (putc(c++, fptr ? fptr : (fptr = fopen(file_name, "w"))) == EOF) // UNDCC_Violation, putc() is called with an expression as the stream argument
    {
      /* Handle error */
    }
  }

  if (fclose(fptr) == EOF)
  {
    /* Handle error */
  }
}
