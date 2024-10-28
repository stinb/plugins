#include <stdio.h>
#include <stdlib.h>

int main(void) {
  FILE *f = fopen("file.txt", "w"); /* UndCC_Violation exit() does not allow
                                       program to determine if an error occurs
                                       while flushing or closing file */
  if (NULL == f) {
    exit(EXIT_FAILURE);
  }
  /* ... */
  exit(EXIT_SUCCESS);
}
