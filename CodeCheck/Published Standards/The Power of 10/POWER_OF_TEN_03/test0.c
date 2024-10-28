#include "test0.h"

static void dynamic()
{
  size_t count = 256;
  int *ptr;

  ptr = malloc(count * sizeof(int));             // UndCC_Violation
  if (ptr)
    free(ptr);                                   // UndCC_Violation

  ptr = calloc(count, sizeof(int));              // UndCC_Violation
  if (ptr) {
    ptr = realloc(ptr, 2 * count * sizeof(int)); // UndCC_Violation
    if (ptr)
      free(ptr);                                 // UndCC_Violation
  }
}
