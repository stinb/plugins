#define DEFINE_GLOBALS
#define DEFINE_GLOBALS_2

#include <stdio.h>
#include "test.h"

// define a global, should not flag violation
int global_int;

int main() {
  
  int a = 2;
  int b = 3;
  
  int sum = a + b;
  
  sample_var = 10;
  
  return 0;
}
