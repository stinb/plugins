#include <stddef.h>

void *malloc( size_t size ) { }
void *calloc( size_t num, size_t size ) { }
void *realloc( void *ptr, size_t new_size ) { }
void free( void *ptr ) { }

static void not_dynamic()
{
  size_t count = 256;
  int *ptr;

  ptr = malloc(count * sizeof(int));             // UndCC_Valid
  if (ptr)
    free(ptr);                                   // UndCC_Valid

  ptr = calloc(count, sizeof(int));              // UndCC_Valid
  if (ptr) {
    ptr = realloc(ptr, 2 * count * sizeof(int)); // UndCC_Valid
    if (ptr)
      free(ptr);                                 // UndCC_Valid
  }
}
