#include <stdio.h>
#include "test.h"

#define MAX 10
#define MIN 1

int alpha;
int beta;          // flag violation, requires space between groupings of declaration types // UndCC_Violation
#define PI 3.14    // flag violation, requires space between groupings of declaration types // UndCC_Violation

int main() {
  
  int omega;
  
  char initial_1 = "a";
  char initial_2 = "f";
  
  char initial_3 = "d";
  
  
  return 0;
}
