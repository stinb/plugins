#include <stdio.h>

int main() {

  int a = 1;
  double b;

  b = (double) a; /* UNDCC_Violation */
  b = static_cast<int>(a); /* UNDCC_Violation */
}
