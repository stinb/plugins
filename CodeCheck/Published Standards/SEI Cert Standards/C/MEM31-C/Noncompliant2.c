#include <stdlib.h>


enum { BUFFER_SIZE = 32 };

int f2(void) {
  char *text_buffer = NULL;
  if (text_buffer == NULL) {
    text_buffer = (char *)malloc(BUFFER_SIZE);  // UndCC_Violation - same as exception but stored in non-static variable
    if (text_buffer == NULL) {
      return -1;
    }
  }
  return 0;
}
