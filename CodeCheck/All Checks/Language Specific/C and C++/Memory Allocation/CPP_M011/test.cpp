#include <iostream>
#include <cstdlib>


int main() {
  
  int* a = NULL;   // Pointer to int, initialize to nothing.
  int* b = NULL;
  int n = 10;
  
  a = new int[n];
  b = new int[5];  // Dyn allocated array b is never freed! Flag a violation // UndCC_Violation
  
  for (int i=0; i<n; i++) {
    a[i] = 0;
  }
  

  delete [] a;  // Valid freeing of memory pointed to by a.
  a = NULL;     // Clear a to prevent using invalid memory reference.

  return 0;
}
