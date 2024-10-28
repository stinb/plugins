// SEI CERT C++


#include <stddef.h>

void *malloc(size_t nbytes) {  // UndCC_Violation
  void *ptr;
  /* Allocate storage from own pool and set ptr */
  return ptr;
}

void free(void *ptr) {  // UndCC_Violation
  /* Return storage to own pool */
}
