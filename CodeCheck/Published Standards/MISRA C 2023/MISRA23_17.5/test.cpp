#include <stdint.h>

/* Intent is that function does not access outside the range array1[0] .. array1[3] */
void fn1(int32_t array1[4]);

/* Intent is that function handles arrays of any size */
void fn2(int32_t array2[]);

/* Dummy implementations to make it compile */
void fn1(int32_t array1[4]) {
  /* Function body - accesses up to 4 elements */
}

void fn2(int32_t array2[]) {
  /* Function body - handles any size array */
}

void fn(int32_t *ptr) {
  int32_t arr3[3] = {1, 2, 3};
  int32_t arr4[4] = {0, 1, 2, 3};
  
  fn1(arr4); // UndCC_Valid
  
  fn1(arr3); // UndCC_Violation
  
  fn1(ptr); // UndCC_Valid
  
  fn2(arr4); // UndCC_Valid
  
  fn2(ptr); // UndCC_Valid
}

int main(void) {
  int32_t test_array[4] = {10, 20, 30, 40};
  fn(test_array);
  return 0;
}
