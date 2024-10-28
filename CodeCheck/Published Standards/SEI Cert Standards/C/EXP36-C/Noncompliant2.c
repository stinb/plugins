#include <assert.h>
#include <string.h>

int *loop_function(void *v_pointer)
{
  /* ... */
  return v_pointer; 
}

void foo(char *char_ptr)
{
  int *int_ptr = loop_function(char_ptr); // UndCC_Violation

  /* ... */
}
