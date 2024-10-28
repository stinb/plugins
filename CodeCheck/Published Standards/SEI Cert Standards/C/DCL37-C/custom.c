// Custom
#include <stddef.h>

// Keywords

#define false 0 // UndCC_Violation


// Reserved identifier patterns

#define __a 0 // UndCC_Violation
#define _A  0 // UndCC_Violation


// Reserved patterns for externally-linked identifiers

float cerf(float) {} // UndCC_Violation

typedef long long unsigned uint333_t; // UndCC_Violation
typedef long long unsigned u333_t;

// (Not including stdlib.h)
void *calloc(size_t num, size_t size); // UndCC_Valid - DCL37-C-EX1
void func(void *ptr) {
  calloc(0, 0);
}
