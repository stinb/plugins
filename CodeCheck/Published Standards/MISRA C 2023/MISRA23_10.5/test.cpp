#include <stdbool.h> // For bool type
#include <stdint.h>  // For int32_t type
#include <stdio.h>   // For printf (optional)

// Define enuma enumeration
enum enuma {
  A,
  B
  // Add enumeration values here
};
enum class enumb {
  C,
  D
};


// Define ena and enc constants
const int ena = 5;
const char enc = 'c';
const float enf = 1.1;
const unsigned int enu = 5;
volatile int p = 2;

int main() {
  enum enuma a = A;
  enum enumb b;
  short s = 10;
  unsigned short us = 10;
  int z = 0;
  (bool) p; // UndCC_Violation int to bool $$$
  (bool) 7; // UndCC_Violation int to bool $$$
  (bool) false; /* Compliant - C99 'false' is essentially Boolean */
  (int32_t) 3U; /* Compliant */
  (char) false; // UndCC_Violation bool to char $$$
  (bool) 'a'; // UndCC_Violation char to bool $$$
  (enum enuma) false; // UndCC_Violation bool to enum $$$
  (int) false; // UndCC_Violation bool to int $$$
  (unsigned int) false; // UndCC_Violation bool to unsigned int $$$
  (float) false; // UndCC_Violation bool to float $$$
  (char) enf; // UndCC_Violation float to char $$$
  (bool) enf; // UndCC_Violation float to bool $$$
  (enum enuma) enc; // UndCC_Violation char to enum $$$
  (enum enuma) b; // UndCC_Violation enumb to enuma (different enums, if they were the same then it would be valid) $$$
  (enum enuma) enu; // UndCC_Violation unsigned int to enum $$$
  (enum enuma) ena; // UndCC_Violation int to enum $$$
  (enum enuma) enf; // UndCC_Violation float to enum $$$
  (float) enc; // UndCC_Violation char to float $$$
  (enum enuma) a; // UndCC_Valid enuma to enuma
  (enuma) a; // UndCC_Valid enuma to enuma
  
  
  (bool) 0;  // UndCC_Valid - by exception 
  (bool) 1; // UndCC_Valid - by exception
  (bool) z; // UndCC_FalsePos - by exception
  (bool) 3U;  // UndCC_Violation
  (int32_t) ena;  // UndCC_Valid
  (enum enuma) 3;  // UndCC_Violation
  (char) enc;  // UndCC_Valid
  (bool) s; // UndCC_Violation short int to bool
  (bool) us; // UndCC_Violation short int to bool

  // Optional: print values to verify
  printf("%d\n", (bool) false); // UndCC_Valid
  printf("%d\n", (int32_t) 3U);  // UndCC_Valid
  printf("%d\n", (bool) 0); // UndCC_Valid
  printf("%d\n", (int32_t) ena); // UndCC_Valid
  printf("%c\n", (char) enc); // UndCC_Valid

  return 0;
}
