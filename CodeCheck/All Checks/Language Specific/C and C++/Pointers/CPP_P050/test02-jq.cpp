#include <stdlib.h>
void fun(int* a) {
}

void foo(int a) {
}

int main() {
  int* a = 0; /* UNDCC_Violation 0 used as null pointer constant */
  int* b = NULL; /* UNDCC_Valid */
  int* c = nullptr; /* UNDCC_Valid */
  
  fun(0); /* UNDCC_Violation 0 used as null pointer constant */
  fun(NULL); /* UNDCC_Valid */
  fun(nullptr); /* UNDCC_Valid */
  
  foo(0); /* UNDCC_Valid 0 used for int type */
  
  return 0;
}
