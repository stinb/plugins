// SEI CERT C

#include <stddef.h>

enum { SIZE = 32 };

void func_bad(void) {
  int nums[SIZE];
  int end;
  int *next_num_ptr = nums;
  size_t free_elements;

  /* Increment next_num_ptr as array fills */

  free_elements = &end - next_num_ptr; // UndCC_Violation
}

void func_good(void) {
  int nums[SIZE];
  int *next_num_ptr = nums;
  size_t free_elements;

  /* Increment next_num_ptr as array fills */

  free_elements = &(nums[SIZE]) - next_num_ptr;
}
