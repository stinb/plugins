#include <stdio.h>

enum
{
  BUFFERSIZE = 32
};

extern void initialize_data(char *data, size_t size);

void func(const char *file_name)
{
  char data[BUFFERSIZE];
  char append_data[BUFFERSIZE];
  FILE *file;

  file = fopen(file_name, "a+");
  if (file == NULL)
  {
    /* Handle error */
  }

  initialize_data(append_data, BUFFERSIZE);

  if (fwrite(append_data, 1, BUFFERSIZE, file) != BUFFERSIZE)
  {
    /* Handle error */
  }
  if (fread(data, 1, BUFFERSIZE, file) < BUFFERSIZE) // UNDCC_Violation, flush or positioning call does not occur between the calls to fread() and fwrite(), but after
  {
    /* Handle there not being data */
  }

  if (fseek(file, 0L, SEEK_SET) != 0)
  {
    /* Handle error */
  }

  if (fclose(file) == EOF)
  {
    /* Handle error */
  }
}
